#include "StdAfx.h"

#include ".\x2stateoption.h"




CX2StateOption::CX2StateOption(void) :
// 2009.01.09 김태완 : 초기화 코드정리
m_pDLGOptionWindow(NULL),
m_pDLGBlackList(NULL),
m_pDLGBlackListAdd(NULL),
m_pDLGBlackListRemoveOKAndCancel(NULL),
m_bIsOptionWindowOpen(false),
//m_OldOptionList,
m_wstrNickNameForBlackListRemove(L""),
m_pResolutionChangeConfirmDialog(NULL),
m_vOriginalResolution(1024.f, 768.f ),			// D3DXVECTOR2
m_bResetOriginalResolution(true)
#ifdef SERV_KEY_MAPPING_INT
, m_DLGWaitKeyConfig(NULL)
, m_fKeyconfigTimer(-1.0f)
, m_eSelectedGameAction( GAME_ACTION_NONE )
#endif // SERV_KEY_MAPPING_INT
{
	// 초기화 코드 ////////////////////////////////////////////////////////////////////////// 
	
// 
// 	m_bIsOptionWindowOpen		= false;
// 
// 	m_wstrNickNameForBlackListRemove = L"";
// 	m_pDLGBlackListRemoveOKAndCancel = NULL;
// 
// 	m_pDLGOptionWindow = NULL;


// 	m_pDLGBlackList = NULL;
// 	m_pDLGBlackListAdd = NULL;
// 
// 	m_pResolutionChangeConfirmDialog = NULL;
// 	m_bResetOriginalResolution = true;

//	m_vOriginalResolution = D3DXVECTOR2( 1024.f, 768.f );

/*
	m_pDLGBlackList = new CKTDGUIDialog( this, L"DLG_SquareUI_BlackList.lua", 0.07f, XDL_OPTION );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBlackList );

	m_pDLGBlackListAdd = new CKTDGUIDialog( this, L"DLG_BlackList_UserSearch.lua", 0.07f, XDL_OPTION );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBlackListAdd );
*/

	//////////////////////////////////////////////////////////////////////////


	m_pDLGOptionWindow = g_pMain->GetMemoryHolder()->GetOptionDLG( (CKTDXStage*) this );
	if( NULL != m_pDLGOptionWindow )
	{
		m_pDLGOptionWindow->SetShow( false );
		m_pDLGOptionWindow->SetEnable( false );
	}


	UpdateBlackListWindow();
	CloseBlackListWindow();
	CloseBlackListAddWindow();
#ifdef BANDICAM_RECORDING
	m_pCamUserWindow=NULL;
#endif BANDICAM_RECORDING
}

CX2StateOption::~CX2StateOption(void)
{

	CloseOptionWindow();
	//SAFE_DELETE_DIALOG( m_pDLGOptionWindow );		// 삭제하면 안됩니다. cx2memoryholder 클래스에서 관리합니다

#ifdef SERV_KEY_MAPPING_INT
	SAFE_DELETE_DIALOG(m_DLGKeyConfig);
	SAFE_DELETE_DIALOG(m_DLGWaitKeyConfig);
#endif // SERV_KEY_MAPPING_INT

	SAFE_DELETE_DIALOG( m_pDLGBlackList );
	SAFE_DELETE_DIALOG( m_pDLGBlackListAdd );
	SAFE_DELETE_DIALOG( m_pDLGBlackListRemoveOKAndCancel );


	SAFE_DELETE( m_pResolutionChangeConfirmDialog );
#ifdef BANDICAM_RECORDING
	SAFE_DELETE_DIALOG(m_pCamUserWindow);
#endif //BANDICAM_RECORDING
}


HRESULT CX2StateOption::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	CX2State::OnFrameMove( fTime, fElapsedTime );

	if( NULL != m_pResolutionChangeConfirmDialog )
	{
		m_pResolutionChangeConfirmDialog->OnFrameMove( fTime, fElapsedTime );

		if( m_pResolutionChangeConfirmDialog->m_fTimeLeft <= 0.f )
		{
			SAFE_DELETE( m_pResolutionChangeConfirmDialog );			
			
			if( true == m_bResetOriginalResolution )
			{
				ResetResolutionToOriginal();
			}
		}
	}

#ifdef SERV_KEY_MAPPING_INT
	if( m_fKeyconfigTimer > 0.0f )
	{
		unsigned char ucUserInput;
		m_fKeyconfigTimer -= fElapsedTime;

		//키 설정부분에서는 ESC및 Enter키 락을 강제로 풀어주어 입력가능하게 한다. 좋은방법은 아니기 때문에 차후 다른방법 강구
		//g_pKTDXApp->GetDIManager()->Getkeyboard()->SetLock(DIK_RETURN, FALSE);
		//g_pKTDXApp->GetDIManager()->Getkeyboard()->SetLock(DIK_ESCAPE, FALSE);
		//키 설정부분에서는 ESC및 Enter키 락을 강제로 풀어주어 입력가능하게 한다. 좋은방법은 아니기 때문에 차후 다른방법 강구
		if( true == g_pKTDXApp->GetDIManager()->ListenDeviceInput( ucUserInput, JOYSTICK )  && m_iInputDevice == JOYSTICK )
		{
			g_pKTDXApp->GetDIManager()->SetKeyChanging( false );
			m_fKeyconfigTimer = -1.0f;

			SetKeyConfig( m_eSelectedGameAction, ucUserInput, JOYSTICK );

			SET_KEYLOCK( m_eSelectedGameAction, TRUE );

			UpdateKeyConfigList();
			SAFE_DELETE_DIALOG(m_DLGWaitKeyConfig);
		}
	}
	else if( NULL != m_DLGWaitKeyConfig )
	{
		SAFE_DELETE_DIALOG(m_DLGWaitKeyConfig);
		m_fKeyconfigTimer = -1.0f;
		m_eSelectedGameAction = GAME_ACTION_NONE;
		g_pKTDXApp->GetDIManager()->SetKeyChanging( false );
	}
#endif // SERV_KEY_MAPPING_INT

	return S_OK;
}

bool CX2StateOption::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return CX2State::MsgProc( hWnd, uMsg, wParam, lParam );
}

bool CX2StateOption::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	CX2GameOption* pGameOption = &g_pMain->GetGameOption();

	switch(wParam)
	{
	case SOUCM_OPTION_WINDOW_CLOSE:
		{
			if( NULL != m_pDLGOptionWindow )
			{
				CloseOptionWindow();
			}
			return true;
		} break;

	case SOUCM_OPTION_GRAPHIC:
		{
			if( NULL != m_pDLGOptionWindow )
			{
				ShowGraphicOption( true );
				ShowSoundOption( false );
#ifndef KEY_MAPPING_INT_IN_KEYBOARD_SETTING
#ifdef SERV_KEY_MAPPING_INT
				ShowInputOption( false );
#endif // SERV_KEY_MAPPING_INT
#endif KEY_MAPPING_INT_IN_KEYBOARD_SETTING
				ShowOtherOption( false );				
				ShowCommunityOption( false );
#ifdef SERV_SECOND_SECURITY
				ShowSecurityOption( false );
#endif SERV_SECOND_SECURITY
#ifdef BANDICAM_RECORDING
				ShowBandiCamOption(false);
#endif BANDICAM_RECORDING
			}
			return true;
		} break;

	case SOUCM_OPTION_SOUND:
		{
			if( NULL != m_pDLGOptionWindow )
			{
				ShowGraphicOption( false );
				ShowSoundOption( true );
#ifndef KEY_MAPPING_INT_IN_KEYBOARD_SETTING
#ifdef SERV_KEY_MAPPING_INT  
				ShowInputOption( false );
#endif // SERV_KEY_MAPPING_INT
#endif KEY_MAPPING_INT_IN_KEYBOARD_SETTING
				ShowOtherOption( false );		
				ShowCommunityOption( false );
#ifdef SERV_SECOND_SECURITY
				ShowSecurityOption( false );
#endif SERV_SECOND_SECURITY
#ifdef BANDICAM_RECORDING
				ShowBandiCamOption(false);
#endif BANDICAM_RECORDING
			}
			return true;
		} break;

#ifndef KEY_MAPPING_INT_IN_KEYBOARD_SETTING
#ifdef SERV_KEY_MAPPING_INT
	case SOUCM_OPTION_INPUT:
		{
			if( NULL != m_pDLGOptionWindow )
			{
				ShowGraphicOption( false );
				ShowSoundOption( false );
				ShowInputOption( true );
				ShowOtherOption( false );		
				ShowCommunityOption( false );
#ifdef SERV_SECOND_SECURITY
				ShowSecurityOption( false );
#endif SERV_SECOND_SECURITY
#ifdef BANDICAM_RECORDING
				ShowBandiCamOption(false);
#endif BANDICAM_RECORDING
			}
			return true;
		} break;
#endif SERV_KEY_MAPPING_INT
#endif KEY_MAPPING_INT_IN_KEYBOARD_SETTING

	case SOUCM_OPTION_OTHER:
		{
			if( NULL != m_pDLGOptionWindow )
			{
				ShowGraphicOption( false );
				ShowSoundOption( false );
#ifndef KEY_MAPPING_INT_IN_KEYBOARD_SETTING
#ifdef SERV_KEY_MAPPING_INT
				ShowInputOption( false );
#endif // SERV_KEY_MAPPING_INT
#endif KEY_MAPPING_INT_IN_KEYBOARD_SETTING
				ShowOtherOption( true );				
				ShowCommunityOption( false );
#ifdef SERV_SECOND_SECURITY
				ShowSecurityOption( false );
#endif SERV_SECOND_SECURITY
#ifdef BANDICAM_RECORDING
				ShowBandiCamOption(false);
#endif BANDICAM_RECORDING
			}
			return true;
		} break;


	case SOUCM_OPTION_COMMUNITY:
		{
			if( NULL != m_pDLGOptionWindow )
			{
				ShowGraphicOption( false );
				ShowSoundOption( false );
				ShowOtherOption( false );				
				ShowCommunityOption( true );
#ifdef SERV_SECOND_SECURITY
				ShowSecurityOption( false );
#endif SERV_SECOND_SECURITY
#ifdef BANDICAM_RECORDING
				ShowBandiCamOption(false);
#endif BANDICAM_RECORDING
			}
			return true;
		} break;

#ifdef SERV_SECOND_SECURITY
	case SOUCM_OPTION_SECURITY:
		{
			if( NULL != m_pDLGOptionWindow )
			{
				ShowGraphicOption( false );
				ShowSoundOption( false );
#ifndef KEY_MAPPING_INT_IN_KEYBOARD_SETTING
#ifdef SERV_KEY_MAPPING_INT
				ShowInputOption( false );
#endif // SERV_KEY_MAPPING_INT
#endif KEY_MAPPING_INT_IN_KEYBOARD_SETTING
				ShowOtherOption( false );				
				ShowCommunityOption( false );
#ifdef SERV_SECOND_SECURITY
				ShowSecurityOption( true );
#endif SERV_SECOND_SECURITY
#ifdef BANDICAM_RECORDING
				ShowBandiCamOption(false);
#endif BANDICAM_RECORDING
			}
			return true;
		} break;
#endif SERV_SECOND_SECURITY


	case SOUCM_OPTION_GRAPHIC_AUTOSET_LEFT:
		{	
			pGameOption->DownAutoOption();
			InitGraphicOption();
		} break;

	case SOUCM_OPTION_GRAPHIC_AUTOSET_RIGHT:
		{
			pGameOption->UpAutoOption();			
			InitGraphicOption();
		} break;


	case SOUCM_OPTION_GRAPHIC_UNIT_HIGH:
		{
			pGameOption->SetUnitDetail( CX2GameOption::OL_HIGH );
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_UNIT_MEDIUM:
		{
			pGameOption->SetUnitDetail( CX2GameOption::OL_MEDIUM );
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_UNIT_LOW:
		{
			pGameOption->SetUnitDetail( CX2GameOption::OL_LOW );
			return true;
		} break; 


	case SOUCM_OPTION_GRAPHIC_TEXTURE_HIGH:
		{
			pGameOption->SetTexDetail( CX2GameOption::OL_HIGH );
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_TEXTURE_MEDIUM:
		{
			pGameOption->SetTexDetail( CX2GameOption::OL_MEDIUM );
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_TEXTURE_LOW:
		{
			pGameOption->SetTexDetail( CX2GameOption::OL_LOW );
			return true;
		} break; 


	case SOUCM_OPTION_GRAPHIC_BACKGROUND_HIGH:
		{
			pGameOption->SetMapDetail( CX2GameOption::OL_HIGH );
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_BACKGROUND_MEDIUM:
		{
			pGameOption->SetMapDetail( CX2GameOption::OL_MEDIUM );
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_BACKGROUND_LOW:
		{
			pGameOption->SetMapDetail( CX2GameOption::OL_LOW );
			return true;
		} break; 


	case SOUCM_OPTION_GRAPHIC_EFFECT_HIGH:
		{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			pGameOption->SetEffectDetail( CX2GameOption::OL_HIGH );
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			pGameOption->SetEffectDetail( true );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_EFFECT_MEDIUM:
		{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			pGameOption->SetEffectDetail( CX2GameOption::OL_MEDIUM );
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			pGameOption->SetEffectDetail( true );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_EFFECT_LOW:
		{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			pGameOption->SetEffectDetail( CX2GameOption::OL_LOW );
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			pGameOption->SetEffectDetail( false );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			return true;
		} break; 
		//{{2011.09.20 이준호 이벤트 처리
#ifdef BANDICAM_RECORDING
	case SOUCM_OPTION_CAM:
		{
			if( NULL != m_pDLGOptionWindow )
			{
				ShowGraphicOption( false );
				ShowSoundOption( false );
#ifndef KEY_MAPPING_INT_IN_KEYBOARD_SETTING
#ifdef SERV_KEY_MAPPING_INT
				ShowInputOption( false );
#endif // SERV_KEY_MAPPING_INT
#endif KEY_MAPPING_INT_IN_KEYBOARD_SETTING
				ShowOtherOption( false );		
				ShowCommunityOption( false );
				ShowBandiCamOption(true);
				ShowSecurityOption( false );
				//g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pCamUserWindow, XDL_NORMAL_1);
				//g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pCamUserWindow, false );
			}
			return true;
		} break;
	case SOUCM_OPTION_VIDEO_GENERAL_LIST:
		{
			CKTDGUIComboBox* pCombo				= (CKTDGUIComboBox*) m_pDLGOptionWindow->GetControl( L"Cam_Option_General_Video_List" );
			if(pCombo==NULL)return false;
			g_pMain->GetGameOption().SetGeneralVideoType(pCombo->GetSelectedItemIndex());

			// 동영상 포맷 설정 변경 시 라디오 버튼 즉시 바뀌도록 수정
			CKTDGUIRadioButton* pRadio	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Cam_Option_Video_User_Rbtn" );
			if(pRadio!=NULL)
				pRadio->SetChecked(false);

			pRadio	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Cam_Option_Video_General_Rbtn" );
			if(pRadio!=NULL)
				pRadio->SetChecked(true);

			g_pMain->GetGameOption().SetUserSetting(false);
			return true;
		}break;
	case SOUCM_OPTION_VIDEO_GENERAL_RBTN:
		{
			g_pMain->GetGameOption().SetUserSetting(false);
			return true;
		}break;
	case SOUCM_OPTION_VIDEO_USER_RBTN:
		{
			g_pMain->GetGameOption().SetUserSetting(true);
			return true;
		}break;
	case SOUCM_OPTION_VIDEO_USER_BTN:
		{
			if ( g_pMain->IsCapturing() == true )
			{	//동영상 촬영중일 때는 설정 할 수 없도록 수정
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_17539 ), g_pMain->GetNowState() );					
				return true;
			}
			else
			{
				g_pMain->GetGameOption().SetUserSetting(true);
				ShowBandiCamOptionUser(true);
			}
			return true;
		}break;
	case SOUCM_OPTION_CAM_USER_ACCEPT_BTN:
		{
			int iVal=30;
			CKTDGUIRadioButton * pRbtn =(CKTDGUIRadioButton *)m_pCamUserWindow->GetControl( L"Cam_User_Quality_Radio_2");
			if(pRbtn==NULL)return false;
			if(pRbtn->GetChecked())
				iVal=70;
			pRbtn =(CKTDGUIRadioButton *)m_pCamUserWindow->GetControl( L"Cam_User_Quality_Radio_3");
			if(pRbtn==NULL)return false;
			if(pRbtn->GetChecked())
				iVal=100;
			g_pMain->GetGameOption().SetVideoQuality(iVal);


			//해상도
			CKTDGUIComboBox* pCombo	= (CKTDGUIComboBox*)m_pCamUserWindow->GetControl( L"Cam_User_Resolution_List");
			if( NULL != pCombo )
			{
				int iVideoSizeW, iVideoSizeH;
				switch(pCombo->GetSelectedItemIndex())
				{
				case 0:		iVideoSizeW=320;	iVideoSizeH=240;	break;
				case 1:		iVideoSizeW=640;	iVideoSizeH=320;	break;
				case 2:		iVideoSizeW=640;	iVideoSizeH=480;	break;
				case 3:		iVideoSizeW=720;	iVideoSizeH=480;	break;
				case 4:		iVideoSizeW=720;	iVideoSizeH=486;	break;
				case 5:		iVideoSizeW=720;	iVideoSizeH=576;	break;
				case 6:		iVideoSizeW=800;	iVideoSizeH=600;	break;
				case 7:		iVideoSizeW=1024;	iVideoSizeH=768;	break;
				case 8:		iVideoSizeW=1152;	iVideoSizeH=864;	break;
				case 9:		iVideoSizeW=1280;	iVideoSizeH=720;	break;
				case 10:	iVideoSizeW=1280;	iVideoSizeH=768;	break;
				case 11:	iVideoSizeW=1280;	iVideoSizeH=800;	break;
				case 12:	iVideoSizeW=1280;	iVideoSizeH=960;	break;
				case 13:	iVideoSizeW=1280;	iVideoSizeH=1024;	break;
				case 14:	iVideoSizeW=1360;	iVideoSizeH=768;	break;
				case 15:	iVideoSizeW=1366;	iVideoSizeH=768;	break;
				case 16:	iVideoSizeW=1440;	iVideoSizeH=900;	break;
				case 17:	iVideoSizeW=1600;	iVideoSizeH=900;	break;
				case 18:	iVideoSizeW=1600;	iVideoSizeH=1024;	break;
				case 19:	iVideoSizeW=1680;	iVideoSizeH=1050;	break;
				default:	iVideoSizeW=1024;	iVideoSizeH=768;	break;
				}
				g_pMain->GetGameOption().SetVideoSizeW( iVideoSizeW );
				g_pMain->GetGameOption().SetVideoSizeH( iVideoSizeH );
				g_pMain->GetGameOption().SetVideoSizeIndex( pCombo->GetSelectedItemIndex() );
			}

			//프레임
			pCombo	= (CKTDGUIComboBox*)m_pCamUserWindow->GetControl( L"Cam_User_Frame_List");
			if( NULL != pCombo )
			{
				switch(pCombo->GetSelectedItemIndex())
				{
#ifdef BANDICAM_OPTION_FIX
				case 0:	g_pMain->GetGameOption().SetVideoFPS(24.0f);	break;
				case 1:	g_pMain->GetGameOption().SetVideoFPS(30.0f);	break;
				case 2:	g_pMain->GetGameOption().SetVideoFPS(50.0f);	break;
				case 3:	g_pMain->GetGameOption().SetVideoFPS(60.0f);	break;
#else //BANDICAM_OPTION_FIX
				case 0:	g_pMain->GetGameOption().SetVideoFPS(15.0f);	break;
				case 1:	g_pMain->GetGameOption().SetVideoFPS(24.0f);	break;
				case 2:	g_pMain->GetGameOption().SetVideoFPS(25.0f);	break;
				case 3:	g_pMain->GetGameOption().SetVideoFPS(30.0f);	break;
				case 4:	g_pMain->GetGameOption().SetVideoFPS(35.0f);	break;
				case 5:	g_pMain->GetGameOption().SetVideoFPS(45.0f);	break;
				case 6:	g_pMain->GetGameOption().SetVideoFPS(50.0f);	break;
				case 7:	g_pMain->GetGameOption().SetVideoFPS(55.0f);	break;
				case 8:	g_pMain->GetGameOption().SetVideoFPS(60.0f);	break;
#endif //BANDICAM_OPTION_FIX
				default:	g_pMain->GetGameOption().SetVideoFPS(60.0f);	break;
				}			
				g_pMain->GetGameOption().SetVideoFPSIndex( pCombo->GetSelectedItemIndex() );//콤보박스 인덱스 기록
			}

			//오디오 음질
			pCombo	= (CKTDGUIComboBox*)m_pCamUserWindow->GetControl( L"Cam_User_AudioQuality_List");
			if( NULL != pCombo )
			{
				switch(pCombo->GetSelectedItemIndex())
				{
#ifdef BANDICAM_OPTION_FIX
				case 0:
					g_pMain->GetGameOption().SetAudioSampleRate(16000);
					break;
				case 1:
					g_pMain->GetGameOption().SetAudioSampleRate(24000);
					break;
				case 2:
					g_pMain->GetGameOption().SetAudioSampleRate(48000);
					break;
#else //BANDICAM_OPTION_FIX
				case 0:
					g_pMain->GetGameOption().SetAudioSampleRate(8000);
					break;
				case 1:
					g_pMain->GetGameOption().SetAudioSampleRate(16000);
					break;
				case 2:
					g_pMain->GetGameOption().SetAudioSampleRate(22000);
					break;
				case 3:
					g_pMain->GetGameOption().SetAudioSampleRate(44100);
					break;
#endif //BANDICAM_OPTION_FIX					
				default:
					g_pMain->GetGameOption().SetAudioSampleRate(0);
					break;
				}			
				g_pMain->GetGameOption().SetAudioSampleRateIndex( pCombo->GetSelectedItemIndex() );//콤보박스 인덱스 기록
			}

			//비디오 포맷
			pCombo	= (CKTDGUIComboBox*)m_pCamUserWindow->GetControl( L"Cam_User_VideoFormat_List");
			if( NULL != pCombo )
			{
				switch(pCombo->GetSelectedItemIndex())
				{
				case 0:
					g_pMain->GetGameOption().SetVideoFormat(FOURCC_MPEG);
					break;
				case 1:
					g_pMain->GetGameOption().SetVideoFormat(FOURCC_MJPG);
					break;
				case 2:
					g_pMain->GetGameOption().SetVideoFormat(FOURCC_MP4V);
					break;
				case 3:
					g_pMain->GetGameOption().SetVideoFormat(FOURCC_ARGB);
					break;
				case 4:
					g_pMain->GetGameOption().SetVideoFormat(FOURCC_RGB24);
					break;
				case 5:
					g_pMain->GetGameOption().SetVideoFormat(FOURCC_YV12);
					break;
				default:
					g_pMain->GetGameOption().SetVideoFormat(FOURCC_MPEG);
					break;
				}
				g_pMain->GetGameOption().SetVideoFormatIndex( pCombo->GetSelectedItemIndex() ); //콤보박스 인덱스 기록
			}

			//오디오 채널
			iVal=2;//기본타입은 스테레오이다
			pRbtn =(CKTDGUIRadioButton *)m_pCamUserWindow->GetControl( L"Cam_User_AudioChannel_Radio_1");
			if( NULL != pRbtn )
			{
				if(pRbtn->GetChecked())
					iVal=1;
				g_pMain->GetGameOption().SetAudioChannels(iVal);
			}
			
			CKTDGUIRadioButton* pRadio			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Cam_Option_Video_User_Rbtn" );
			if( NULL != pRadio )
			{
				pRadio->SetChecked(true);
			}

			pRadio		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Cam_Option_Video_General_Rbtn" );
			if( NULL != pRadio )
			{
				pRadio->SetChecked(false);
			}			

			ShowBandiCamOptionUser(false);

			return true;
		}break;
	case SOUCM_OPTION_CAM_USER_WINDOW_CLOSE:
		{
			ShowBandiCamOptionUser(false);
			return true;
		}break;
#endif BANDICAM_RECORDING
			//}}
	case SOUCM_OPTION_GRAPHIC_FIELD_HIGH:
		{
			pGameOption->SetFieldDetail( CX2GameOption::FL_HIGH );
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_FIELD_MEDIUM:
		{
			pGameOption->SetFieldDetail( CX2GameOption::FL_MEDIUM );			
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_FIELD_LOW:
		{
			pGameOption->SetFieldDetail( CX2GameOption::FL_LOW );			
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_FIELD_PARTY_ON:
		{			
			CKTDGUIRadioButton* pRadio_Graphic_Field_Low			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldLow" );
			CKTDGUIRadioButton* pRadio_Graphic_Field_Middle			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldMiddle" );
			CKTDGUIRadioButton* pRadio_Graphic_Field_High			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldHigh" );

			pGameOption->SetFieldParty(true);
			pRadio_Graphic_Field_Low->SetEnable(false);
			pRadio_Graphic_Field_Middle->SetEnable(false);
			pRadio_Graphic_Field_High->SetEnable(false);			
			return true;
		} break; 
	case SOUCM_OPTION_GRAPHIC_FIELD_PARTY_OFF:
		{
			CKTDGUIRadioButton* pRadio_Graphic_Field_Low			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldLow" );
			CKTDGUIRadioButton* pRadio_Graphic_Field_Middle			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldMiddle" );
			CKTDGUIRadioButton* pRadio_Graphic_Field_High			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldHigh" );

			pGameOption->SetFieldParty(false);
			pRadio_Graphic_Field_Low->SetEnable(true);
			pRadio_Graphic_Field_Middle->SetEnable(true);
			pRadio_Graphic_Field_High->SetEnable(true);			
			
			return true;
		} break; 
		//{{ 09.03.21 김태완 : SD모드 옵션에 추가
	case SOUCM_OPTION_GRAPHIC_FIELD_SD_ON:
		{
			pGameOption->SetFieldSD(true);
// 			if(g_pData != NULL)
// 			{
// 				g_pData->SetShowSDUnit( true );
// 			}
		} break;
	case SOUCM_OPTION_GRAPHIC_FIELD_SD_OFF:
		{
			pGameOption->SetFieldSD(false);
// 			if(g_pData != NULL)
// 			{
// 				g_pData->SetShowSDUnit( false );
// 			}
		} break;
		//}}

	
	case SOUCM_OPTION_GRAPHIC_RESOLUTION:
		{
			m_vOriginalResolution = g_pMain->GetGameOption().GetOptionList().m_vResolution;
			
			//{{ dmlee 2008.06.03 해상도 변경할지 물어보는 팝업창 (20초동안)
			m_bResetOriginalResolution = true;
			SAFE_DELETE( m_pResolutionChangeConfirmDialog );
			m_pResolutionChangeConfirmDialog = new ConfirmDialog( GET_STRING( STR_ID_673 ), 20.f,
				SOUCM_OPTION_GRAPHIC_CHANGE_RESOLUTION_OK, SOUCM_OPTION_GRAPHIC_CHANGE_RESOLUTION_CANCEL, (CKTDXStage*) this );
			//}} dmlee 2008.06.03 해상도 변경할지 물어보는 팝업창 (20초동안)


			CKTDGUIComboBox* pCombo_Graphic_Resoultion = (CKTDGUIComboBox*) m_pDLGOptionWindow->GetControl( L"Option_Resolution" );


			DXUTDeviceSettings deviceSettings = DXUTGetDeviceSettings();
			DWORD dwResolution = PtrToUlong( pCombo_Graphic_Resoultion->GetSelectedData() );

			DWORD dwWidth = LOWORD( dwResolution );
			DWORD dwHeight = HIWORD( dwResolution );

			pGameOption->SetResolution( dwWidth, dwHeight );

			return true;
		}
		break;

	case SOUCM_OPTION_SOUND_SOUND_ON:
		{
			pGameOption->SetSound( true );
			return true;
		} break;

	case SOUCM_OPTION_SOUND_SOUND_OFF:
		{
			pGameOption->SetSound( false );
		} break;

	case SOUCM_OPTION_SOUND_BGM_ON:
		{
			pGameOption->SetMusic( true );
			return true;
		} break;

	case SOUCM_OPTION_SOUND_BGM_OFF:
		{
			pGameOption->SetMusic( false );
			return true;
		} break;

	case SOUCM_OPTION_SOUND_ENABLE_3D_SOUND_ON:
		{
			pGameOption->SetEnable3DSound( true );

			if( NULL != g_pX2Game &&
				NULL != g_pX2Game->GetWorld() )
			{
				g_pX2Game->GetWorld()->Play3DEffectBGM( true );
			}

			if( NULL != g_pTFieldGame &&
				NULL != g_pTFieldGame->GetWorld() )
			{
				g_pTFieldGame->GetWorld()->Play3DEffectBGM( true );
			}

			return true;

		} break;

	case SOUCM_OPTION_SOUND_ENABLE_3D_SOUND_OFF:
		{
			pGameOption->SetEnable3DSound( false );

			if( NULL != g_pX2Game &&
				NULL != g_pX2Game->GetWorld() )
			{
				g_pX2Game->GetWorld()->Play3DEffectBGM( false );
			}

			if( NULL != g_pTFieldGame &&
				NULL != g_pTFieldGame->GetWorld() )
			{
				g_pTFieldGame->GetWorld()->Play3DEffectBGM( false );
			}
			return true;

		} break;

		
	case SOUCM_OPTION_SOUND_SOUND_VOLUME:
		{
			CKTDGUISlider* pSlider_Volume = (CKTDGUISlider*) m_pDLGOptionWindow->GetControl( L"Slider_Sound_Volume" );
			int iValue = pSlider_Volume->GetValue();
//			if( iValue == 0 )
//				iValue = -1;
			if( iValue > -500 )
				iValue = -500;
			
			pGameOption->SetSoundVolume( (float)iValue );			

			// fix!! 에서 [-3000,0] 사이의 값을 옵션에서 입력으로 받는다
			const float MAGIC_BOUND = 3000.f;
			float fVolume = MAGIC_BOUND + g_pKTDXApp->GetDSManager()->GetSoundVolume();
			fVolume = (float) ( ( (int)fVolume ) % (int)MAGIC_BOUND );
			fVolume /= MAGIC_BOUND;

			if( NULL != g_pX2Game &&
				NULL != g_pX2Game->GetWorld() )
			{
				g_pX2Game->GetWorld()->SetEffectBGMVolume( fVolume );
			}

			if( NULL != g_pTFieldGame &&
				NULL != g_pTFieldGame->GetWorld() )
			{
				g_pTFieldGame->GetWorld()->SetEffectBGMVolume( fVolume );
			}
			
			return true;
		}		
		break;

	case SOUCM_OPTION_SOUND_BGM_VOLUME:				
		{
			CKTDGUISlider* pSlider_Volume = (CKTDGUISlider*) m_pDLGOptionWindow->GetControl( L"Slider_BGM_Volume" );
			int iValue = pSlider_Volume->GetValue();
//			if( iValue == 0 )
//				iValue = -1;
			if( iValue > -500 )
				iValue = -500;

			pGameOption->SetMusicVolume( (float)iValue );
			return true;
		}		
		break;

	case SOUCM_OPTION_OTHER_DYNAMIC_CAMERA_ON:
		{
			pGameOption->SetDynamicCamera( true );
			return true;
		} break;

	case SOUCM_OPTION_OTHER_DYNAMIC_CAMERA_OFF:
		{
			pGameOption->SetDynamicCamera( false );
			return true;
		} break;


	case SOUCM_OPTION_OTHER_PLAY_GUIDE_ON:
		{
			pGameOption->SetPlayGuide( true );
			return true;
		} break;

	case SOUCM_OPTION_OTHER_PLAY_GUIDE_OFF:
		{
			pGameOption->SetPlayGuide( false );
			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_DENY_FRIENDSHIP_ON:
		{
			pGameOption->SetDenyFriendship( CX2GameOption::BL_ON );

			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_DENY_FRIENDSHIP_OFF:
		{
			pGameOption->SetDenyFriendship( CX2GameOption::BL_OFF );
			
			return true;
		} break;


	case SOUCM_OPTION_COMMUNITY_DENY_WHISPER_ON:
		{
			pGameOption->SetDenyInviteGuild( CX2GameOption::BL_ON );
			
			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_DENY_WHISPER_OFF:
		{
			pGameOption->SetDenyInviteGuild( CX2GameOption::BL_OFF );
			
			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_DENY_WHISPER_ONLY_FRIENDS:
		{
			pGameOption->SetDenyInviteGuild( CX2GameOption::BL_ONLY_FRIEND );
			
			return true;
		} break;


	case SOUCM_OPTION_COMMUNITY_DENY_PARTY_ON:
		{
			pGameOption->SetDenyParty( CX2GameOption::BL_ON );
			
			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_DENY_PARTY_OFF:
		{
			pGameOption->SetDenyParty( CX2GameOption::BL_OFF );
			
			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_DENY_PARTY_ONLY_FRIENDS:
		{
			pGameOption->SetDenyParty( CX2GameOption::BL_ONLY_FRIEND );
			
			return true;
		} break;


	case SOUCM_OPTION_COMMUNITY_DENY_PERSONAL_TRADE_ON:
		{
			pGameOption->SetDenyPersonalTrade( CX2GameOption::BL_ON );
			
			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_DENY_PERSONAL_TRADE_OFF:
		{
			pGameOption->SetDenyPersonalTrade( CX2GameOption::BL_OFF );
			
			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_DENY_PERSONAL_TRADE_ONLY_FRIENDS:
		{
			pGameOption->SetDenyPersonalTrade( CX2GameOption::BL_ONLY_FRIEND );
			
			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_OPEN_BLACKLIST:
		{
			OpenBlackListWindow();
			return true;

		} break;

	case SOUCM_OPTION_COMMUNITY_CLOSE_BLACKLIST:
		{
			CloseBlackListWindow();
			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_BLACKLIST_ADD:
		{
			OpenBlackListAddWindow();
			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_BLACKLIST_ADD_OK:
		{
			if( NULL != m_pDLGBlackListAdd )
			{
				CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*) m_pDLGBlackListAdd->GetControl( L"Edit_NickName" );
				wstring wstrNickName = pEditBox->GetText();
				if( 0 != wstrNickName.length() )
				{
					//Handler_EGS_NEW_BLACKLIST_USER_REQ( wstrNickName );
				}
			}

			CloseBlackListAddWindow();
			return true;

		} break;
	case SOUCM_OPTION_COMMUNITY_BLACKLIST_ADD_CANCEL:
		{
			CloseBlackListAddWindow();
			return true;
		} break;

	case SOUCM_OPTION_COMMUNITY_BLACKLIST_REMOVE:
		{
			CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGBlackList->GetControl( L"ListBoxChatContent" );
			if( NULL != pListBox )
			{
				CKTDGUIListBox::ListBoxItem* pSelectedItem = pListBox->GetSelectedItem();
				if( NULL != pSelectedItem )
				{
					m_wstrNickNameForBlackListRemove = pSelectedItem->strText;

					SAFE_DELETE_DIALOG( m_pDLGBlackListRemoveOKAndCancel );
					
					m_pDLGBlackListRemoveOKAndCancel = 
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_674, "L", m_wstrNickNameForBlackListRemove ) ), SOUCM_OPTION_COMMUNITY_BLACKLIST_REMOVE_OK, (CKTDXStage*) this, SOUCM_OPTION_COMMUNITY_BLACKLIST_REMOVE_CANCEL );
				}
			}
			return true;

		} break;

	case SOUCM_OPTION_COMMUNITY_BLACKLIST_REMOVE_OK:
		{
			//Handler_EGS_DEL_BLACKLIST_USER_REQ( m_wstrNickNameForBlackListRemove );
			m_wstrNickNameForBlackListRemove = L"";
			return true;

		} break;

	case SOUCM_OPTION_COMMUNITY_BLACKLIST_REMOVE_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBlackListRemoveOKAndCancel, NULL, false );
			m_pDLGBlackListRemoveOKAndCancel = NULL;
		}
		break;

	case SOUCM_OPTION_GRAPHIC_CHANGE_RESOLUTION_OK:
		{
			if( NULL != m_pResolutionChangeConfirmDialog )
				m_pResolutionChangeConfirmDialog->m_fTimeLeft = 0.f;

			m_bResetOriginalResolution = false;

			return true;
		} break;

	case SOUCM_OPTION_GRAPHIC_CHANGE_RESOLUTION_CANCEL:
		{
			if( NULL != m_pResolutionChangeConfirmDialog )
				m_pResolutionChangeConfirmDialog->m_fTimeLeft = 0.f;

			m_bResetOriginalResolution = true;

			return true;
		} break;

	case SOUCM_OPTION_OTHER_CHARINFO_LEVEL_ON:
		pGameOption->SetCharInfoLevel( true );
		break;
	case SOUCM_OPTION_OTHER_CHARINFO_LEVEL_OFF:
		pGameOption->SetCharInfoLevel( false );
		break;
	case SOUCM_OPTION_OTHER_CHARINFO_PVP_ON:
		pGameOption->SetCharInfoPvp( true );
		break;
	case SOUCM_OPTION_OTHER_CHARINFO_PVP_OFF:
		pGameOption->SetCharInfoPvp( false );
		break;
	case SOUCM_OPTION_OTHER_CHARINFO_GUILD_ON:
		pGameOption->SetCharInfoGuild( true );
		break;
	case SOUCM_OPTION_OTHER_CHARINFO_GUILD_OFF:
		pGameOption->SetCharInfoGuild( false );
		break;
	case SOUCM_OPTION_OTHER_CHARINFO_NOTHING_ON:
		pGameOption->SetCharInfoNothing( true );
		break;
	case SOUCM_OPTION_OTHER_CHARINFO_NOTHING_OFF:
		pGameOption->SetCharInfoNothing( false );
		break;

	case SOUCM_OPTION_OTHER_PARTY_VETO_ON:
		pGameOption->SetRefuseParty(true);
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;
	case SOUCM_OPTION_OTHER_PARTY_VETO_OFF:
		pGameOption->SetRefuseParty(false);
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;
	case SOUCM_OPTION_OTHER_FRIEND_VETO_ON:
		pGameOption->SetRefuseFriend(true);
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;
	case SOUCM_OPTION_OTHER_FRIEND_VETO_OFF:
		pGameOption->SetRefuseFriend(false);
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;
	case SOUCM_OPTION_OTHER_PERSONAL_TRADE_VETO_ON:
		pGameOption->SetRefusePersonalTrade(true);
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;
	case SOUCM_OPTION_OTHER_PERSONAL_TRADE_VETO_OFF:
		pGameOption->SetRefusePersonalTrade(false);
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;

		//{{ kimhc // 2009-10-09 // 길드초대 거부 추가
#ifdef	GUILD_MANAGEMENT
	case SOUCM_OPTION_OTHER_GUILD_VETO_ON:
		pGameOption->SetRefuseGuildInvitation( true );
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;
			
	case SOUCM_OPTION_OTHER_GUILD_VETO_OFF:
		pGameOption->SetRefuseGuildInvitation( false );
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-10-09 // 길드초대 거부 추가

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case SOUCM_OPTION_OTHER_COUPLE_VETO_ON:
		pGameOption->SetRefuseRequestCouple( true );
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;

	case SOUCM_OPTION_OTHER_COUPLE_VETO_OFF:
		pGameOption->SetRefuseRequestCouple( false );
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM


#ifdef FIX_INVITE_PVP_PLAYER // 김태환
	case SOUCM_OPTION_OTHER_INVITE_PVP_ON:
		pGameOption->SetRefuseInvitePVP( true );
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;

	case SOUCM_OPTION_OTHER_INVITE_PVP_OFF:
		pGameOption->SetRefuseInvitePVP( false );
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
		break;
#endif // FIX_INVITE_PVP_PLAYER


#ifdef SERV_LOCAL_RANKING_SYSTEM
	case SOUCM_OPTION_RANKUPUI_ON:
		pGameOption->SetShowRankUpInDungeon( true );
		break;

	case SOUCM_OPTION_RANKUPUI_OFF:
		pGameOption->SetShowRankUpInDungeon( false );
		break;
#endif //SERV_LOCAL_RANKING_SYSTEM


	case SOUCM_OPTION_OTHER_CHECK_FULLSCREEN:
		{
			CKTDGUICheckBox* pCheckWindowMode = (CKTDGUICheckBox*) lParam;
			pGameOption->SetFullScreen( !pCheckWindowMode->GetChecked() );
			return true;

		} break;
#ifdef SERV_SECOND_SECURITY

	case SOUCM_OPTION_SECURITY_SETTING:
		{
#ifdef SERV_SECOND_SECURITY_K3R_AUTH
			g_pMain->GetSecurityPad()->Handler_ECH_SECURITY_AUTH_REQ();
#else SERV_SECOND_SECURITY_K3R_AUTH
			g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPS_CREATE_PASSWORD);
#endif SERV_SECOND_SECURITY_K3R_AUTH
		}break;
	case SOUCM_OPTION_SECURITY_CHANGE_PASSWORD:
		{
			g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPS_CHANGE_PASSWORD);
		}break;
	case SOUCM_OPTION_SECURITY_RELEASE:
		{
			g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPS_RELEASE_PASSWORD);
		}break;
#endif SERV_SECOND_SECURITY

#ifdef SERV_KEY_MAPPING_INT
	case SOUCM_KEY_CONFIG_OPEN:
		{
			if( g_pKTDXApp->GetDIManager()->GetJoystic() != NULL )
				g_pKTDXApp->GetDIManager()->GetJoystic()->GetActionKeys( m_mapGamePadActionKeys );
			ShowKeyConfigDLG(true);
		}break;
	case SOUCM_KEY_CONFIG_CLOSE_SAVE:
		{
			Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ();
			ShowKeyConfigDLG(false);
		}break;
	case SOUCM_KEY_CONFIG_CLOSE_CANCEL:
		{
			if( g_pKTDXApp->GetDIManager()->GetJoystic() != NULL )
				g_pKTDXApp->GetDIManager()->GetJoystic()->SetActionKeys( m_mapGamePadActionKeys );
			ShowKeyConfigDLG(false);
		}break;
	case SOUCM_JOY_CONFIG_BUTTON_CLICKED:
		{
			m_DLGWaitKeyConfig = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_4671), g_pMain->GetNowState() );
			m_fKeyconfigTimer = 10.0f;

			CKTDGUIButton* pButtonKeyconfig	= (CKTDGUIButton*)lParam;
			m_eSelectedGameAction = MappingKeyConfigIndex( pButtonKeyconfig->GetDummyInt(0) );
			g_pKTDXApp->GetDIManager()->SetKeyChanging( true );
			m_iInputDevice = JOYSTICK;
		} break;
	case SOUCM_JOY_CONFIG_BUTTON_RCLICKED:
		{
			CKTDGUIButton* pButtonKeyconfig	= (CKTDGUIButton*)lParam;
			m_eSelectedGameAction = MappingKeyConfigIndex( pButtonKeyconfig->GetDummyInt(0) );
			SetKeyConfig( m_eSelectedGameAction, DIK_UP, JOYSTICK );
			UpdateKeyConfigList();
		} break;
	case SOUCM_KEY_CONFIG_ENABLE:
		{
			g_pKTDXApp->GetDIManager()->SetEnableJoyStick(true);
			pGameOption->SetJoyEnable(true);
		} break;
	case SOUCM_KEY_CONFIG_DISABLE:
		{
			g_pKTDXApp->GetDIManager()->SetEnableJoyStick(false);
			pGameOption->SetJoyEnable(false);
		} break;
	case SOUCM_KEY_CONFIG_RESET:
		{
			g_pKTDXApp->GetDIManager()->SetGamePadDefaultMap();
			UpdateKeyConfigList();
		} break;
#endif // SERV_KEY_MAPPING_INT

#ifdef VERTICAL_SYNC_OPTION
	case SOUCM_OPTION_GRAPHIC_FLASH_EFFECT:
		{
			CKTDGUICheckBox* pCheckFlashEffect = (CKTDGUICheckBox*) lParam;
			pGameOption->SetFlashEffect( pCheckFlashEffect->GetChecked() );
			return true;
		} break;
	
	case SOUCM_OPTION_OHTER_VERTICAL_SYNC_ON:
		{
			pGameOption->SetVerticalSync( true );
			return true;
		} break;

	case SOUCM_OPTION_OHTER_VERTICAL_SYNC_OFF:
		{
			pGameOption->SetVerticalSync( false );
			return true;
		} break;
#endif
	case SOUCM_OPTION_OHTER_SCSS_ONESELF:
		{
			pGameOption->SetStateCutinShowState(CX2GameOption::SCSS_ONESELF);
			return true;
		} break;
	case SOUCM_OPTION_OHTER_SCSS_PARTY_MEMBER:
		{
			pGameOption->SetStateCutinShowState(CX2GameOption::SCSS_PARTY_MEMBER);
			return true;
		} break;
	case SOUCM_OPTION_OHTER_SCSS_HIDE:
		{
			pGameOption->SetStateCutinShowState(CX2GameOption::SCSS_HIDE);
			return true;
		} break;
#ifdef SKILL_SLOT_UI_TYPE_B
	case SOUCM_OPTION_SKILL_UI_TYPE_A:
		{
			pGameOption->SetIsSkillUITypeA(true);
		} break;
	case SOUCM_OPTION_SKILL_UI_TYPE_B:
		{
			pGameOption->SetIsSkillUITypeA(false);
		} break;
#endif //SKILL_SLOT_UI_TYPE_B
#ifdef IDENTITY_CONFIRM
	case SOUCM_OPTION_IDENTITY:
		{
			ShellExecute(NULL, L"open", GET_STRING(STR_ID_11306), L"", L"", SW_SHOW );
		} break;
#endif IDENTITY_CONFIRM
	}

	return false;
}

bool CX2StateOption::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	switch( wParam )
	{
	case EGS_UPDATE_COMMUNITY_OPTION_ACK:
		{
			return Handler_EGS_UPDATE_COMMUNITY_OPTION_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	}


	return false;
}


bool CX2StateOption::CloseOptionWindow()
{
	CloseBlackListWindow();
	CloseBlackListAddWindow();

#if 0 
	CX2GameOption::OptionList* pGameOptionList = &g_pMain->GetGameOption().GetOptionList();

	bool bChangeCommunityOption = false;
	if( m_OldOptionList.m_eDenyFriendship != pGameOptionList->m_eDenyFriendship )
	{
		bChangeCommunityOption = true;
	}
	else if( m_OldOptionList.m_eDenyWhisper != pGameOptionList->m_eDenyWhisper )
	{
		bChangeCommunityOption = true;
	}
	else if( m_OldOptionList.m_eDenyParty != pGameOptionList->m_eDenyParty )
	{
		bChangeCommunityOption = true;
	}
	else if( m_OldOptionList.m_eDenyPersonalTrade != pGameOptionList->m_eDenyPersonalTrade )
	{
		bChangeCommunityOption = true;
	}
	else if( m_OldOptionList.m_mapBlackList != pGameOptionList->m_mapBlackList )
	{
		bChangeCommunityOption = true;
	}

	if( true == bChangeCommunityOption )
	{
		Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ();
	}
#endif

	if( NULL == m_pDLGOptionWindow )
		return false;

	m_bIsOptionWindowOpen = false;


	m_pDLGOptionWindow->SetShow( false );
	m_pDLGOptionWindow->SetEnable( false );
	m_pDLGOptionWindow->SetModal( true );

#ifdef SERV_KEY_MAPPING_INT
	SAFE_DELETE_DIALOG(m_DLGKeyConfig);
	SAFE_DELETE_DIALOG(m_DLGWaitKeyConfig);
#endif // SERV_KEY_MAPPING_INT

	g_pMain->GetGameOption().SaveScriptFile();

	//m_OldOptionList = *pGameOptionList;
#ifdef BANDICAM_RECORDING
	ShowBandiCamOptionUser(false);
#endif BANDICAM_RECORDING

	return true;
}

bool CX2StateOption::OpenOptionWindow()
{
	if( NULL == m_pDLGOptionWindow )
		return false;

	m_pDLGOptionWindow->SetPos( D3DXVECTOR2(303.f, 202.f) );

	CX2GameOption::OptionList* pGameOptionList = &g_pMain->GetGameOption().GetOptionList();
	//m_OldOptionList = *pGameOptionList;

	m_pDLGOptionWindow->SetShow( true );
	m_pDLGOptionWindow->SetEnable( true );
	m_pDLGOptionWindow->SetModal( true );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGOptionWindow, true );

	InitGraphicOption();
	InitSoundOption();
#ifndef KEY_MAPPING_INT_IN_KEYBOARD_SETTING
#ifdef SERV_KEY_MAPPING_INT
	InitInputOption();
#endif // SERV_KEY_MAPPING_INT
#endif KEY_MAPPING_INT_IN_KEYBOARD_SETTING
	InitOtherOption();
	//InitCommunityOption();

	CKTDGUICheckBox* pCheckBox_OptionGraphic	= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_Graphic_Menu" );
	CKTDGUICheckBox* pCheckBox_OptionSound		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_Sound_Menu" );
#ifndef KEY_MAPPING_INT_IN_KEYBOARD_SETTING
#ifdef SERV_KEY_MAPPING_INT
	CKTDGUICheckBox* pCheckBox_OptionInput		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_Input_Menu" );
#endif // SERV_KEY_MAPPING_INT
#endif KEY_MAPPING_INT_IN_KEYBOARD_SETTING
	CKTDGUICheckBox* pCheckBox_OptionOther		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_Other_Menu" );
#if defined( SERV_SECOND_SECURITY ) && !defined( SERV_DISABLE_SECOND_SECURITY )	// 해외팀 수정
	#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		CKTDGUICheckBox* pCheckBox_OptionSecurity	= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_security_Menu" );
		pCheckBox_OptionSecurity->SetShowEnable(true, true);
	#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
#endif
	//CKTDGUICheckBox* pCheckBox_OptionCommunity	= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_Community_Menu" );

	ShowGraphicOption(	pCheckBox_OptionGraphic->GetChecked() );
	ShowSoundOption(	pCheckBox_OptionSound->GetChecked() );
#ifndef KEY_MAPPING_INT_IN_KEYBOARD_SETTING
#ifdef SERV_KEY_MAPPING_INT
	ShowInputOption(	pCheckBox_OptionInput->GetChecked() );
#endif // SERV_KEY_MAPPING_INT
#endif KEY_MAPPING_INT_IN_KEYBOARD_SETTING
	ShowOtherOption(	pCheckBox_OptionOther->GetChecked() );
#if defined( SERV_SECOND_SECURITY ) && !defined( SERV_DISABLE_SECOND_SECURITY )	// 해외팀 수정
	#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		ShowSecurityOption(	pCheckBox_OptionSecurity->GetChecked() );
	#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
#endif
	//ShowCommunityOption( pCheckBox_OptionCommunity->GetChecked() );

#ifdef BANDICAM_RECORDING
	CKTDGUICheckBox* pCheckBox	= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_Cam_Menu" );
	ShowBandiCamOption(pCheckBox->GetChecked());
#endif BANDICAM_RECORDING

	m_bIsOptionWindowOpen = true;
	return true;
}

#ifdef SERV_SECOND_SECURITY
bool CX2StateOption::OpenSecurityOptionWindow()
{
	if( NULL == m_pDLGOptionWindow )
		return false;

	m_pDLGOptionWindow->SetPos( D3DXVECTOR2(303.f, 202.f) );

	//CX2GameOption::OptionList* pGameOptionList = &g_pMain->GetGameOption().GetOptionList();
	//m_OldOptionList = *pGameOptionList;

	m_pDLGOptionWindow->SetShow( true );
	m_pDLGOptionWindow->SetEnable( true );
	m_pDLGOptionWindow->SetModal( true );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGOptionWindow, true );

	InitGraphicOption();
	InitSoundOption();
	InitOtherOption();

	CKTDGUICheckBox* pCheckBox_OptionGraphic	= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_Graphic_Menu" );
	CKTDGUICheckBox* pCheckBox_OptionSound		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_Sound_Menu" );
	CKTDGUICheckBox* pCheckBox_OptionOther		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_Other_Menu" );
#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	CKTDGUICheckBox* pCheckBox_OptionSecurity	= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_security_Menu" );
	pCheckBox_OptionSecurity->SetShowEnable(true, true);
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

	pCheckBox_OptionGraphic->SetChecked(false);
	pCheckBox_OptionSound->SetChecked(false);
	pCheckBox_OptionOther->SetChecked(false);
#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	pCheckBox_OptionSecurity->SetChecked(true);
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
	

#ifdef BANDICAM_RECORDING
	CKTDGUICheckBox* pCheckBox_OptionCam		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_Cam_Menu" );
	if( NULL != pCheckBox_OptionCam )
	{
		pCheckBox_OptionCam->SetChecked(false);
	}

	ShowBandiCamOption( false );
#endif BANDICAM_RECORDING

	ShowGraphicOption( false );
	ShowSoundOption( false );
	ShowOtherOption( false );
	ShowSecurityOption(	true );

#ifndef KEY_MAPPING_INT_IN_KEYBOARD_SETTING
#ifdef SERV_KEY_MAPPING_INT
	CKTDGUICheckBox* pCheckBox_OptionInput		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Option_Input_Menu" );
	if( NULL != pCheckBox_OptionInput )
	{
		pCheckBox_OptionInput->SetChecked(false);
	}

	ShowInputOption( false );
#endif // SERV_KEY_MAPPING_INT
#endif KEY_MAPPING_INT_IN_KEYBOARD_SETTING

	m_bIsOptionWindowOpen = true;
	return true;
}
#endif SERV_SECOND_SECURITY



void CX2StateOption::ShowGraphicOption( bool bShow )
{
	CKTDGUIStatic* pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Option_Window_Graphic" );
	if ( pStatic_Graphic != NULL )
		pStatic_Graphic->SetShow( bShow );

	ShowStaticGraphic(bShow);
/*
	for( int i=0; i<20; i++ )
	{
		pStatic_Graphic->GetPicture(i)->SetShow( bShow );		
	}
*/
	CKTDGUIButton* pButton_Grahpic_AutoSetLevel_Left		= (CKTDGUIButton*) m_pDLGOptionWindow->GetControl( L"ButtonLeft_AutoSetLevel" );
	CKTDGUIButton* pButton_Grahpic_AutoSetLevel_Right		= (CKTDGUIButton*) m_pDLGOptionWindow->GetControl( L"ButtonRight_AutoSetLevel" );

	CKTDGUIRadioButton* pRadio_Graphic_Unit_High			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Unit_High" );
	CKTDGUIRadioButton* pRadio_Graphic_Unit_Middle			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Unit_Middle" );
	CKTDGUIRadioButton* pRadio_Graphic_Unit_Low				= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Unit_Low" );

	CKTDGUIRadioButton* pRadio_Graphic_Texture_High			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Texture_High" );
	CKTDGUIRadioButton* pRadio_Graphic_Texture_Middle		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Texture_Middle" );
	CKTDGUIRadioButton* pRadio_Graphic_Texture_Low			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Texture_Low" );

	CKTDGUIRadioButton* pRadio_Graphic_Background_High		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_BackGround_High" );
	CKTDGUIRadioButton* pRadio_Graphic_Background_Middle	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_BackGround_Middle" );
	CKTDGUIRadioButton* pRadio_Graphic_Background_Low		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_BackGround_Low" );

	CKTDGUIRadioButton* pRadio_Graphic_Effect_High			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Effect_High" );
	CKTDGUIRadioButton* pRadio_Graphic_Effect_Middle		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Effect_Middle" );
	CKTDGUIRadioButton* pRadio_Graphic_Effect_Low			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Effect_Low" );

	CKTDGUIComboBox* pCombo_Graphic_Resoultion				= (CKTDGUIComboBox*) m_pDLGOptionWindow->GetControl( L"Option_Resolution" );

	CKTDGUIRadioButton* pRadio_Graphic_Field_Low			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldLow" );
	CKTDGUIRadioButton* pRadio_Graphic_Field_Middle			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldMiddle" );
	CKTDGUIRadioButton* pRadio_Graphic_Field_High			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldHigh" );

	//CKTDGUICheckBox* pCheckBox_Graphic_FieldFriend			= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"FieldFriend" );
	CKTDGUICheckBox* pCheckBox_Graphic_FieldParty			= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"FieldParty" );
	CKTDGUICheckBox* pCheckBox_Graphic_FieldSD				= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"FieldSD" );

	pButton_Grahpic_AutoSetLevel_Left->SetShow( bShow );
	pButton_Grahpic_AutoSetLevel_Right->SetShow( bShow );

	pRadio_Graphic_Field_Low->SetShowEnable(bShow, bShow);
	pRadio_Graphic_Field_Middle->SetShowEnable(bShow, bShow);
	pRadio_Graphic_Field_High->SetShowEnable(bShow, bShow);
	pCheckBox_Graphic_FieldParty->SetShowEnable(bShow, bShow);
	pCheckBox_Graphic_FieldSD->SetShowEnable(bShow, bShow);


	CX2GameOption::OptionList* pOptionList = &g_pMain->GetGameOption().GetOptionList() ;

	switch( pOptionList->m_MaxUnitDetail )
	{
	case CX2GameOption::OL_HIGH:
		{			
			pRadio_Graphic_Unit_High->SetShow( bShow );
			pRadio_Graphic_Unit_High->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(4)->SetShow( bShow );

			pRadio_Graphic_Unit_Middle->SetShow( bShow );
			pRadio_Graphic_Unit_Middle->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(5)->SetShow( bShow );

			pRadio_Graphic_Unit_Low->SetShow( bShow );
			pRadio_Graphic_Unit_Low->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(6)->SetShow( bShow );
		} break;

	case CX2GameOption::OL_MEDIUM:
		{
			pRadio_Graphic_Unit_High->SetShow( false );
			pRadio_Graphic_Unit_High->SetEnable( false );
			//pStatic_Graphic->GetPicture(4)->SetShow( false );

			pRadio_Graphic_Unit_Middle->SetShow( bShow );
			pRadio_Graphic_Unit_Middle->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(5)->SetShow( bShow );

			pRadio_Graphic_Unit_Low->SetShow( bShow );
			pRadio_Graphic_Unit_Low->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(6)->SetShow( bShow );
		} break;

	case CX2GameOption::OL_LOW:
		{
			pRadio_Graphic_Unit_High->SetShow( false );
			pRadio_Graphic_Unit_High->SetEnable( false );
			//pStatic_Graphic->GetPicture(4)->SetShow( false );

			pRadio_Graphic_Unit_Middle->SetShow( false );
			pRadio_Graphic_Unit_Middle->SetEnable( false );
			//pStatic_Graphic->GetPicture(5)->SetShow( false );

			pRadio_Graphic_Unit_Low->SetShow( bShow );
			pRadio_Graphic_Unit_Low->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(6)->SetShow( bShow );
		}
	}


	// 텍스쳐
	switch( pOptionList->m_MaxTexDetail )
	{
	case CX2GameOption::OL_HIGH:
		{			
			pRadio_Graphic_Texture_High->SetShow( bShow );
			pRadio_Graphic_Texture_High->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(8)->SetShow( bShow );

			pRadio_Graphic_Texture_Middle->SetShow( bShow );
			pRadio_Graphic_Texture_Middle->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(9)->SetShow( bShow );

			pRadio_Graphic_Texture_Low->SetShow( bShow );
			pRadio_Graphic_Texture_Low->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(10)->SetShow( bShow );
		} break;

	case CX2GameOption::OL_MEDIUM:
		{
			pRadio_Graphic_Texture_High->SetShow( false );
			pRadio_Graphic_Texture_High->SetEnable( false );
			//pStatic_Graphic->GetPicture(8)->SetShow( false );

			pRadio_Graphic_Texture_Middle->SetShow( bShow );
			pRadio_Graphic_Texture_Middle->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(9)->SetShow( bShow );

			pRadio_Graphic_Texture_Low->SetShow( bShow );
			pRadio_Graphic_Texture_Low->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(10)->SetShow( bShow );
		} break;

	case CX2GameOption::OL_LOW:
		{
			pRadio_Graphic_Texture_High->SetShow( false );
			pRadio_Graphic_Texture_High->SetEnable( false );
			//pStatic_Graphic->GetPicture(8)->SetShow( false );

			pRadio_Graphic_Texture_Middle->SetShow( false );
			pRadio_Graphic_Texture_Middle->SetEnable( false );
			//pStatic_Graphic->GetPicture(9)->SetShow( false );

			pRadio_Graphic_Texture_Low->SetShow( bShow );
			pRadio_Graphic_Texture_Low->SetEnable( bShow );
			//pStatic_Graphic->GetPicture(10)->SetShow( bShow );
		}
	}


	// 배경
	switch( pOptionList->m_MaxTexDetail )
	{
	case CX2GameOption::OL_HIGH:
		{	
			pRadio_Graphic_Background_High->SetShowEnable( bShow, bShow );	
			pRadio_Graphic_Background_Middle->SetShowEnable( bShow, bShow );	
			pRadio_Graphic_Background_Low->SetShowEnable( bShow, bShow );	
		} break;

	case CX2GameOption::OL_MEDIUM:
		{
			pRadio_Graphic_Background_High->SetShowEnable( false, false );	
			pRadio_Graphic_Background_Middle->SetShowEnable( bShow, bShow );	
			pRadio_Graphic_Background_Low->SetShowEnable( bShow, bShow );	

		} break;

	case CX2GameOption::OL_LOW:
		{
			pRadio_Graphic_Background_High->SetShowEnable( false, false );	
			pRadio_Graphic_Background_Middle->SetShowEnable( false, false );	
			pRadio_Graphic_Background_Low->SetShowEnable( bShow, bShow );	

		} break;
	}

	if(pOptionList->m_bParty == true)
	{
		pRadio_Graphic_Field_Low->SetEnable(false);
		pRadio_Graphic_Field_Middle->SetEnable(false);
		pRadio_Graphic_Field_High->SetEnable(false);
	}
	else
	{
		pRadio_Graphic_Field_Low->SetEnable(true);
		pRadio_Graphic_Field_Middle->SetEnable(true);
		pRadio_Graphic_Field_High->SetEnable(true);
	}

	pRadio_Graphic_Effect_High->SetShow( bShow );	
	pRadio_Graphic_Effect_Middle->SetShow( bShow );	
	pRadio_Graphic_Effect_Low->SetShow( bShow );		

	pCombo_Graphic_Resoultion->SetShow( bShow );


	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_TRAINING_GAME:
		{
			pRadio_Graphic_Texture_High->SetEnable( false );		
			pRadio_Graphic_Texture_Middle->SetEnable( false );	
			pRadio_Graphic_Texture_Low->SetEnable( false );		
		} break;
	}

	//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
	// 기타 옵션에서 옮겨옴
	CKTDGUIStatic* pStatic_WindowMode = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Static_WindowMode" );

	if( NULL != pStatic_WindowMode )
	{
		pStatic_WindowMode->SetShow( bShow );
	}

	CKTDGUICheckBox* pCheckWindowMode = (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Window_Mode" );
	if( NULL != pCheckWindowMode )
	{
		pCheckWindowMode->SetShow( bShow );
	}
#endif	CHAT_WINDOW_IMPROV
	//}} kimhc // 2010.3.12 //	채팅창 개편

#ifdef VERTICAL_SYNC_OPTION
	CKTDGUICheckBox* pCheckFlashEffect = (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"FlashEffect" );
	if ( NULL != pCheckFlashEffect )
	{
		pCheckFlashEffect->SetShow( bShow );
	}
#endif
}

void CX2StateOption::ShowSoundOption( bool bShow )
{
	CKTDGUIStatic* pStatic_Sound = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Option_Window_Sound" );
	pStatic_Sound->SetShow( bShow );

	ShowStaticSound(bShow);
/*
	for( int i=0; i<10; i++ )
	{
		pStatic_Sound->GetPicture(i)->SetShow( bShow );		
	}
*/
//	CKTDGUIRadioButton* pRadio_Sound_On		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Sound_On" );
	//CKTDGUIRadioButton* pRadio_Sound_Off	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Sound_Off" );

//	CKTDGUIRadioButton* pRadio_BGM_On		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_BGM_On" );
//	CKTDGUIRadioButton* pRadio_BGM_Off		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_BGM_Off" );

	CKTDGUICheckBox* pCheckBoxSoundOff		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"NoSound" );
	CKTDGUICheckBox* pCheckBoxEffectSoundOff = (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"NoEffect_Sound" );

	CKTDGUISlider* pSlide_Sound_Volume		= (CKTDGUISlider*) m_pDLGOptionWindow->GetControl( L"Slider_Sound_Volume" );
	CKTDGUISlider* pSlide_BGM_Volume		= (CKTDGUISlider*) m_pDLGOptionWindow->GetControl( L"Slider_BGM_Volume" );



	CX2GameOption::OptionList* pOptionList = &g_pMain->GetGameOption().GetOptionList() ;
	if( NULL != pOptionList )
	{
		CKTDGUIRadioButton* pRadioEnable3DSoundOn	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"3DSound_on" );
		CKTDGUIRadioButton* pRadioEnable3DSoundOff	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"3DSound_off" );
		if( NULL != pRadioEnable3DSoundOn )
		{
			if( true == pOptionList->m_bEnable3DSound )
			{
				pRadioEnable3DSoundOn->SetChecked(true);
			}
			pRadioEnable3DSoundOn->SetShow( bShow );
		}

		if( NULL != pRadioEnable3DSoundOff )
		{
			if( false == pOptionList->m_bEnable3DSound )
			{
				pRadioEnable3DSoundOff->SetChecked(true);
			}
			pRadioEnable3DSoundOff->SetShow( bShow );
		}
	}


	

	pCheckBoxSoundOff->SetShow( bShow );
	pCheckBoxEffectSoundOff->SetShow( bShow );

	pSlide_Sound_Volume->SetShow( bShow );
	pSlide_BGM_Volume->SetShow( bShow );

}


void CX2StateOption::ShowOtherOption( bool bShow )
{
	CKTDGUIStatic* pStatic_Other = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Option_Window_Other" );
	CKTDGUIStatic* pStatic_Other1 = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Charac_Info" );
	CKTDGUIStatic* pStatic_Other2 = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"g_pStaticSign" );	
	CKTDGUIStatic* pStatic_Other3 = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Charac_Level" );
	CKTDGUIStatic* pStatic_Other4 = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"pvp_rank" );
	//CKTDGUIStatic* pStatic_Other5 = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Static_Guild" );
	CKTDGUIStatic* pStatic_Other6 = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Info_hide" );
	CKTDGUIStatic* pStatic_Other7 = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"StaticPartyVeto" );
	CKTDGUIStatic* pStatic_Other8 = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"StaticFriendVeto" );
	CKTDGUIStatic* pStatic_Other9 = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Privacy" );
	CKTDGUIStatic* pStatic_Other10 = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"StaticTradeVeto" );	



	pStatic_Other->SetShow( bShow );
	pStatic_Other1->SetShow( bShow );
	pStatic_Other2->SetShow( bShow );	
	pStatic_Other3->SetShow( bShow );
	pStatic_Other4->SetShow( bShow );
	//pStatic_Other5->SetShow( bShow );
	pStatic_Other6->SetShow( bShow );
	pStatic_Other7->SetShow( bShow );
	pStatic_Other8->SetShow( bShow );
	pStatic_Other9->SetShow( bShow );
	pStatic_Other10->SetShow( bShow );


	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGOptionWindow->GetControl(L"SkillCutin"));
	pStatic->SetShow(bShow);

	//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifndef	CHAT_WINDOW_IMPROV
	CKTDGUIStatic* pStatic_WindowMode = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Static_WindowMode" );

	if( NULL != pStatic_WindowMode )
	{
		pStatic_WindowMode->SetShow( bShow );
	}
#endif	CHAT_WINDOW_IMPROV
	//}} kimhc // 2010.3.12 //	채팅창 개편	


	//{{ kimhc // 2009-10-09 // 길드초대 거부 추가
#ifdef	GUILD_MANAGEMENT
	CKTDGUIStatic* pStatic_Other11 = NULL;
	pStatic_Other11 = static_cast< CKTDGUIStatic* >( m_pDLGOptionWindow->GetControl( L"StaticGuildVeto" ) );

	if ( pStatic_Other11 != NULL )
		pStatic_Other11->SetShow( bShow );
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-10-09 // 길드초대 거부 추가

#ifdef SERV_LOCAL_RANKING_SYSTEM
	CKTDGUIStatic* pStatic_Other12 = static_cast<CKTDGUIStatic*>( m_pDLGOptionWindow->GetControl( L"DungeonRank_Hide" ) );
	if( NULL != pStatic_Other12 )
		pStatic_Other12->SetShow( bShow );
#endif //SERV_LOCAL_RANKING_SYSTEM

#ifdef ADDED_RELATIONSHIP_SYSTEM		/// 커플신청 거부 추가
	CKTDGUIStatic* pStatic_Other13 = NULL;
	pStatic_Other13 = static_cast< CKTDGUIStatic* >( m_pDLGOptionWindow->GetControl( L"StaticCoupleVeto" ) );

	if ( pStatic_Other13 != NULL )
		pStatic_Other13->SetShow( bShow );
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef FIX_INVITE_PVP_PLAYER // 김태환	/// 대전 초대 거부 추가
	CKTDGUIStatic* pStatic_Other14 = NULL;
	pStatic_Other14 = static_cast< CKTDGUIStatic* >( m_pDLGOptionWindow->GetControl( L"StaticFightVeto" ) );

	if ( pStatic_Other14 != NULL )
		pStatic_Other14->SetShow( bShow );
#endif // FIX_INVITE_PVP_PLAYER

	ShowStaticOther(bShow);
/*
	for( int i=0; i<6; i++ )
	{
		pStatic_Other->GetPicture(i)->SetShow( bShow );		
	}
*/

	CKTDGUIRadioButton* pRadio_Dynamic_Camera_On	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Dynamic_Camera_On" );
	CKTDGUIRadioButton* pRadio_Dynamic_Camera_Off	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Dynamic_Camera_Off" );

	CKTDGUIRadioButton* pRadio_Play_Guide_On		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Play_Guide_On" );
	CKTDGUIRadioButton* pRadio_Play_Guide_Off		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Play_Guide_Off" );


	pRadio_Dynamic_Camera_On->SetShow( bShow );
	pRadio_Dynamic_Camera_Off->SetShow( bShow );

	pRadio_Play_Guide_On->SetShow( bShow );
	pRadio_Play_Guide_Off->SetShow( bShow );

#ifdef VERTICAL_SYNC_OPTION
	CKTDGUIRadioButton* pRadio_Vertical_Sync_On		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Vertical_Sync_On" );
	CKTDGUIRadioButton* pRadio_Vertical_Sync_Off	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Vertical_Sync_Off" );

	pRadio_Vertical_Sync_On->SetShow( bShow );
	pRadio_Vertical_Sync_Off->SetShow( bShow );
#endif

	CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*> (m_pDLGOptionWindow->GetControl( L"Option_SkillCutin_OneSelf" ));
	pRadio->SetShow( bShow );
	pRadio = static_cast<CKTDGUIRadioButton*> (m_pDLGOptionWindow->GetControl( L"Option_SkillCutin_PartyMember" ));
	pRadio->SetShow( bShow );
	pRadio = static_cast<CKTDGUIRadioButton*> (m_pDLGOptionWindow->GetControl( L"Option_SkillCutin_Hide" ));
	pRadio->SetShow( bShow );

#ifdef SKILL_SLOT_UI_TYPE_B
	pStatic = static_cast<CKTDGUIStatic*>(m_pDLGOptionWindow->GetControl(L"SkillSlotUI"));
	if( NULL != pStatic )
		pStatic->SetShow(bShow);

	pRadio = static_cast<CKTDGUIRadioButton*> (m_pDLGOptionWindow->GetControl( L"Option_SkillUI_TypeA" ));
	if( NULL != pRadio )
		pRadio->SetShow(bShow);
	pRadio = static_cast<CKTDGUIRadioButton*> (m_pDLGOptionWindow->GetControl( L"Option_SkillUI_TypeB" ));
	if( NULL != pRadio )
		pRadio->SetShow(bShow);
#endif //SKILL_SLOT_UI_TYPE_B

	CKTDGUICheckBox* pCheckBoxLevel		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Level" );
	CKTDGUICheckBox* pCheckBoxPvpRank		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"PVP" );
	//CKTDGUICheckBox* pCheckBoxGuild		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"CheckBox_Guild" );
	CKTDGUICheckBox* pCheckBoxNothing		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Nothing" );
#ifdef SERV_LOCAL_RANKING_SYSTEM
	CKTDGUICheckBox* pCheckBoxRankUp		= static_cast<CKTDGUICheckBox*>( m_pDLGOptionWindow->GetControl( L"DungeonRank" ) );
	if( NULL != pCheckBoxRankUp )
		pCheckBoxRankUp->SetShow( bShow );
#endif //SERV_LOCAL_RANKING_SYSTEM
	pCheckBoxLevel->SetShow( bShow );
	pCheckBoxPvpRank->SetShow( bShow );
	//pCheckBoxGuild->SetShow( bShow );
	pCheckBoxNothing->SetShow( bShow );

	CKTDGUICheckBox* pCheckPartyVeto		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"party_veto" );
	CKTDGUICheckBox* pCheckFriendVeto		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"friend_veto" );
	CKTDGUICheckBox* pCheckTradeVeto		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Trade_veto" );

	pCheckPartyVeto->SetShow( bShow );
	pCheckFriendVeto->SetShow( bShow );
	pCheckTradeVeto->SetShow( bShow );

	//{{ kimhc // 2009-10-09 // 길드초대 거부 추가
#ifdef	GUILD_MANAGEMENT
	CKTDGUICheckBox* pCheckRefuseGuildInvitation		= NULL;
	pCheckRefuseGuildInvitation = static_cast< CKTDGUICheckBox* >( m_pDLGOptionWindow->GetControl( L"guild_veto" ) );

	if ( pCheckRefuseGuildInvitation != NULL )
		pCheckRefuseGuildInvitation->SetShow( bShow );
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-10-09 // 길드초대 거부 추가

#ifdef ADDED_RELATIONSHIP_SYSTEM		/// 커플신청 거부 추가
	CKTDGUICheckBox* pCheckCoupleVeto					= NULL;
	pCheckCoupleVeto			= static_cast< CKTDGUICheckBox* >( m_pDLGOptionWindow->GetControl( L"couple_veto" ) );

	if ( pCheckCoupleVeto != NULL )
		pCheckCoupleVeto->SetShow( bShow );
#endif // ADDED_RELATIONSHIP_SYSTEM


//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifndef	CHAT_WINDOW_IMPROV
	CKTDGUICheckBox* pCheckWindowMode = (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Window_Mode" );
	if( NULL != pCheckWindowMode )
	{
		pCheckWindowMode->SetShow( bShow );
	}
#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.12 //	채팅창 개편	

#ifdef FIX_INVITE_PVP_PLAYER // 김태환		/// 대전 초대 거부 추가
	CKTDGUICheckBox* pCheckInvitePVPVeto	= NULL;
	pCheckInvitePVPVeto						= static_cast< CKTDGUICheckBox* >( m_pDLGOptionWindow->GetControl( L"Fight_veto" ) );

	if ( pCheckInvitePVPVeto != NULL )
		pCheckInvitePVPVeto->SetShow( bShow );
#endif // FIX_INVITE_PVP_PLAYER

#ifdef IDENTITY_CONFIRM
	CKTDGUIStatic* pStatic_Identity = NULL; //2011.04.27 lygan_조성욱 //방침미 관련 스트링 추가 처리
	pStatic_Identity			= (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Identity_Confirm_Name" );
	pStatic_Identity->SetShowEnable(bShow, bShow);

	CKTDGUIButton* pButtonIdentity = reinterpret_cast< CKTDGUIButton* >( m_pDLGOptionWindow->GetControl( L"Identity_Confirm" ) );
	ASSERT(pButtonIdentity);
	pButtonIdentity->SetShowEnable(bShow, bShow);
#endif IDENTITY_CONFIRM
}

#ifdef SERV_SECOND_SECURITY
void CX2StateOption::ShowSecurityOption( bool bShow )
{
	CKTDGUIStatic* pStatic_Security = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Option_Window_Security" );
	pStatic_Security->SetShow( bShow );

	ShowStaticSecurity(bShow);

	CKTDGUIButton* pButtonSecuritySetting		= (CKTDGUIButton*) m_pDLGOptionWindow->GetControl( L"ButtonSecuritySetting" );
	CKTDGUIButton* pButtonSecurityChange		= (CKTDGUIButton*) m_pDLGOptionWindow->GetControl( L"ButtonSecurityChange" );
	CKTDGUIButton* pButtonSecurityRelease		= (CKTDGUIButton*) m_pDLGOptionWindow->GetControl( L"ButtonSecurityRelease" );


	pButtonSecuritySetting->SetShowEnable( bShow, bShow );
	pButtonSecurityChange->SetShowEnable( bShow, bShow );
	pButtonSecurityRelease->SetShowEnable( bShow, bShow );

	if(true == bShow &&
		NULL != g_pMain->GetSecurityPad())
	{
		if(true == g_pMain->GetSecurityPad()->GetUseSecondPW())
		{
			pButtonSecuritySetting->SetEnable( false );
		}
		else
		{
			pButtonSecurityChange->SetEnable( false );
			pButtonSecurityRelease->SetEnable( false );
		}
	}
}
#endif SERV_SECOND_SECURITY


void CX2StateOption::ShowStaticGraphic( bool bShow )
{
	CKTDGUIStatic* pStatic_Graphic = NULL;
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"auto" );
	pStatic_Graphic->SetShowEnable(bShow, bShow);
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Option_Window_Graphic_Num" );
	pStatic_Graphic->SetShowEnable(bShow, bShow);
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"you" );
	pStatic_Graphic->SetShowEnable(bShow, bShow);
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"texture" );
	pStatic_Graphic->SetShowEnable(bShow, bShow);
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"background" );
	pStatic_Graphic->SetShowEnable(bShow, bShow);
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"effect" );
	pStatic_Graphic->SetShowEnable(bShow, bShow);
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"limite" );
	pStatic_Graphic->SetShowEnable(bShow, bShow);
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"OptionParty" );
	pStatic_Graphic->SetShowEnable(bShow, bShow);
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"OptionSD" );
	pStatic_Graphic->SetShowEnable(bShow, bShow);
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"resolusion" );
	pStatic_Graphic->SetShowEnable(bShow, bShow);
#ifdef VERTICAL_SYNC_OPTION
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"OptionEffect" );
	pStatic_Graphic->SetShowEnable(bShow, bShow);
#endif
}
void CX2StateOption::ShowStaticSound( bool bShow )
{
	CKTDGUIStatic* pStatic_Sound = NULL;
	pStatic_Sound = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"bgm" );
	pStatic_Sound->SetShowEnable(bShow, bShow);
	pStatic_Sound = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"bgmoff" );
	pStatic_Sound->SetShowEnable(bShow, bShow);
	pStatic_Sound = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"effectSound" );
	pStatic_Sound->SetShowEnable(bShow, bShow);
	pStatic_Sound = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"effectoff" );
	pStatic_Sound->SetShowEnable(bShow, bShow);
	pStatic_Sound = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"3dsound" );
	pStatic_Sound->SetShowEnable(bShow, bShow);
	pStatic_Sound = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"static_3DSound_on" );
	pStatic_Sound->SetShowEnable(bShow, bShow);
	pStatic_Sound = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"static_3DSound_off" );
	pStatic_Sound->SetShowEnable(bShow, bShow);
}
void CX2StateOption::ShowStaticOther( bool bShow )
{
	CKTDGUIStatic* pStatic_Other = NULL;
	pStatic_Other = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"auto_Camera" );
	pStatic_Other->SetShowEnable(bShow, bShow);
	pStatic_Other = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"onCamera" );
	pStatic_Other->SetShowEnable(bShow, bShow);
	pStatic_Other = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"offCamera" );
	pStatic_Other->SetShowEnable(bShow, bShow);
	pStatic_Other = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"auto_Guide" );
	pStatic_Other->SetShowEnable(bShow, bShow);
	pStatic_Other = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"onGuide" );
	pStatic_Other->SetShowEnable(bShow, bShow);
	pStatic_Other = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"offGuide" );
	pStatic_Other->SetShowEnable(bShow, bShow);
#ifdef VERTICAL_SYNC_OPTION
	pStatic_Other = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"FRAME" );
	pStatic_Other->SetShowEnable(bShow, bShow);
	pStatic_Other = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"onVerticalSync" );
	pStatic_Other->SetShowEnable(bShow, bShow);
	pStatic_Other = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"offVerticalSync" );
	pStatic_Other->SetShowEnable(bShow, bShow);
#endif
}
//{{2011.09.21 이준호 반디캠 옵션 설정 메소드 추가 X_x88
#ifdef BANDICAM_RECORDING 
void CX2StateOption::ShowBandiCamOption( bool bShow )
{

	CKTDGUIStatic* pStatic_Graphic = NULL;
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Cam_Option_Window" );
	if(pStatic_Graphic==NULL)
		return;
	if(pStatic_Graphic!=NULL)
		pStatic_Graphic->SetShowEnable(bShow, bShow);
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Cam_Option_Title1" );
	if(pStatic_Graphic!=NULL)
		pStatic_Graphic->SetShowEnable(bShow, bShow);
	pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Cam_Option_Title2" );
	if(pStatic_Graphic!=NULL)
		pStatic_Graphic->SetShowEnable(bShow, bShow);


	CKTDGUIComboBox* pCombo				= (CKTDGUIComboBox*) m_pDLGOptionWindow->GetControl( L"Cam_Option_General_Video_List" );
	if(pCombo!=NULL)
	{
		pCombo->SetShowEnable(bShow,bShow);

		if(pCombo->GetNumItems()==0)
		{
			pCombo->AddItem(GET_STRING( STR_ID_14405 ),(LPVOID) NULL, false);
			pCombo->AddItem(GET_STRING( STR_ID_14406 ),(LPVOID) NULL, false);
			pCombo->AddItem(GET_STRING( STR_ID_14407 ),(LPVOID) NULL, false);
			pCombo->AddItem(GET_STRING( STR_ID_14408 ),(LPVOID) NULL, false);
			pCombo->AddItem(GET_STRING( STR_ID_14409 ),(LPVOID) NULL, false);
			pCombo->AddItem(GET_STRING( STR_ID_14410 ),(LPVOID) NULL, false);
			pCombo->AddItem(GET_STRING( STR_ID_14411 ),(LPVOID) NULL, false);
			pCombo->AddItem(GET_STRING( STR_ID_14412 ),(LPVOID) NULL, false);
		}
	}
	
	CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Cam_Option_Video_General_Rbtn" );
	if(pRadio!=NULL)
	{
		pRadio->SetChecked(!g_pMain->GetGameOption().GetUserSetting());
		pRadio->SetShowEnable(bShow,bShow);
	}
	pRadio			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Cam_Option_Video_User_Rbtn" );
	if(pRadio!=NULL)
	{
		pRadio->SetChecked(g_pMain->GetGameOption().GetUserSetting());
		pRadio->SetShowEnable(bShow,bShow);
	}

	CKTDGUIButton * pBTN=(CKTDGUIButton *)m_pDLGOptionWindow->GetControl(L"Cam_Option_Video_User_Btn");
	if(pBTN!=NULL)
		pBTN->SetShowEnable(bShow,bShow);
}
void CX2StateOption::ShowBandiCamOptionUser( bool bShow )
{

	CKTDGUIStatic* pStatic_Graphic = NULL;
	
	if(m_pCamUserWindow==NULL && bShow == true)
	{
		m_pCamUserWindow = new CKTDGUIDialog( this, L"DLG_UI_Option_Cam_Ex.lua", 0.07f, XDL_POP_UP );
		assert(m_pCamUserWindow);
		
		if( m_pCamUserWindow == NULL )
			return;

		m_pCamUserWindow->SetPos(D3DXVECTOR2(358,228));//여기에 옵션에 위치를 조정하면 베이스 옵션창 위치도 수정해야 한다 
		//그래야지 창 꼬이는 문제를 해결할수있음
		m_pCamUserWindow->SetSize(430,320);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pCamUserWindow );

		//여기서 사이즈를 추가하면 위에서 이벤트가 일어나는 부분에서 직접 스트링을 추가해 주어야 합니다(이벤트 처리부분 윗부분을 참고하세요
		//만약 설정하지 않으면 디폴트 값이 들어갑니다

		//해상도
		CKTDGUIComboBox* pCombo	= (CKTDGUIComboBox*)m_pCamUserWindow->GetControl( L"Cam_User_Resolution_List");
		if(pCombo!=NULL)
		{
			pCombo->AddItem(L"320 X 240",(LPVOID) NULL, false);
			pCombo->AddItem(L"640 X 320",(LPVOID) NULL, false);
			pCombo->AddItem(L"640 X 480",(LPVOID) NULL, false);
			pCombo->AddItem(L"720 X 480",(LPVOID) NULL, false);
			pCombo->AddItem(L"720 X 486",(LPVOID) NULL, false);
			pCombo->AddItem(L"720 X 576",(LPVOID) NULL, false);
			pCombo->AddItem(L"800 X 600",(LPVOID) NULL, false);
			pCombo->AddItem(L"1024 X 768",(LPVOID) NULL, false);
			pCombo->AddItem(L"1152 X 864",(LPVOID) NULL, false);
			pCombo->AddItem(L"1280 X 720",(LPVOID) NULL, false);
			pCombo->AddItem(L"1280 X 768",(LPVOID) NULL, false);
			pCombo->AddItem(L"1280 X 800",(LPVOID) NULL, false);
			pCombo->AddItem(L"1280 X 960",(LPVOID) NULL, false);
			pCombo->AddItem(L"1280 X 1024",(LPVOID) NULL, false);
			pCombo->AddItem(L"1360 X 768",(LPVOID) NULL, false);
			pCombo->AddItem(L"1366 X 768",(LPVOID) NULL, false);
			pCombo->AddItem(L"1440 X 900",(LPVOID) NULL, false);
			pCombo->AddItem(L"1600 X 900",(LPVOID) NULL, false);
			pCombo->AddItem(L"1600 X 1024",(LPVOID) NULL, false);
			pCombo->AddItem(L"1680 X 1050",(LPVOID) NULL, false);
			
			if( NULL != g_pMain )
			{	//새로 생성 되어도 기존 값 유지 할 수 있도록 수정.
				pCombo->SetSelectedByIndex( g_pMain->GetGameOption().GetVideoSizeIndex() ) ;
			}
		}

		//프레임
		pCombo	= (CKTDGUIComboBox*)m_pCamUserWindow->GetControl( L"Cam_User_Frame_List");
		if(pCombo!=NULL)
		{
#ifdef BANDICAM_OPTION_FIX
			pCombo->AddItem(L"24.0fps",(LPVOID) NULL, false);
			pCombo->AddItem(L"30.0fps",(LPVOID) NULL, false);
			pCombo->AddItem(L"50.0fps",(LPVOID) NULL, false);
			pCombo->AddItem(L"60.0fps",(LPVOID) NULL, false);
#else //BANDICAM_OPTION_FIX
			pCombo->AddItem(L"15.0fps",(LPVOID) NULL, false);
			pCombo->AddItem(L"24.0fps",(LPVOID) NULL, false);
			pCombo->AddItem(L"25.0fps",(LPVOID) NULL, false);
			pCombo->AddItem(L"30.0fps",(LPVOID) NULL, false);
			pCombo->AddItem(L"35.0fps",(LPVOID) NULL, false);
			pCombo->AddItem(L"40.0fps",(LPVOID) NULL, false);
			pCombo->AddItem(L"45.0fps",(LPVOID) NULL, false);
			pCombo->AddItem(L"50.0fps",(LPVOID) NULL, false);
			pCombo->AddItem(L"60.0fps",(LPVOID) NULL, false);
#endif //BANDICAM_OPTION_FIX
			if( NULL != g_pMain )
			{	//새로 생성 되어도 기존 값 유지 할 수 있도록 수정.
				pCombo->SetSelectedByIndex( g_pMain->GetGameOption().GetVideoFPSIndex() ) ;
			}
		}

		//음질
		pCombo	= (CKTDGUIComboBox*)m_pCamUserWindow->GetControl( L"Cam_User_AudioQuality_List");
		if(pCombo!=NULL)
		{
#ifdef BANDICAM_OPTION_FIX	
			pCombo->AddItem(L"16000k",(LPVOID) NULL, false);
			pCombo->AddItem(L"24000K",(LPVOID) NULL, false);
			pCombo->AddItem(L"48000k",(LPVOID) NULL, false);
#else //BANDICAM_OPTION_FIX
			pCombo->AddItem(L"8000k",(LPVOID) NULL, false);
			pCombo->AddItem(L"16000k",(LPVOID) NULL, false);
			pCombo->AddItem(L"22000K",(LPVOID) NULL, false);
			pCombo->AddItem(L"44000k",(LPVOID) NULL, false);
#endif //BANDICAM_OPTION_FIX
			if( NULL != g_pMain )
			{	//새로 생성 되어도 기존 값 유지 할 수 있도록 수정.
				pCombo->SetSelectedByIndex( g_pMain->GetGameOption().GetAudioSampleRateIndex() ) ;
			}
		}
		
		//오디오 채널
		switch( g_pMain->GetGameOption().GetAudioChannels() ) 
		{
		case 1: //모노
			{
				CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*) m_pCamUserWindow->GetControl( L"Cam_User_AudioChannel_Radio_1" ); //모노
				if( NULL != pRadio )
				{
					pRadio->SetChecked(true);
				}
			} break;
		case 2: //스테레오
		default:
			{
				CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*) m_pCamUserWindow->GetControl( L"Cam_User_AudioChannel_Radio_2" ); //스테레오
				if( NULL != pRadio )
				{
					pRadio->SetChecked(true);
				}
			}
			break;
		}
		
		//화질
		switch( g_pMain->GetGameOption().GetVideoQuality() ) 
		{ // SetVideoQuality 할때 값을 30, 70, 100으로 고정하고 있습니다.
		case 30:
			{
				CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*) m_pCamUserWindow->GetControl( L"Cam_User_Quality_Radio_1" ); //저화질
				if( NULL != pRadio )
				{
					pRadio->SetChecked(true);
				}
			}
			break;
		case 70:
			{
				CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*) m_pCamUserWindow->GetControl( L"Cam_User_Quality_Radio_2" ); //일반화질
				if( NULL != pRadio )
				{
					pRadio->SetChecked(true);
				}
			}
			break;
		case 100:
			{
				CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*) m_pCamUserWindow->GetControl( L"Cam_User_Quality_Radio_3" ); //고화질
				if( NULL != pRadio )
				{
					pRadio->SetChecked(true);
				}
			}
			break;
		default:
			{
				CKTDGUIRadioButton* pRadio = (CKTDGUIRadioButton*) m_pCamUserWindow->GetControl( L"Cam_User_Quality_Radio_2" ); //일반화질
				if( NULL != pRadio )
				{
					pRadio->SetChecked(true);
				}
			}
			break;
		}
		
		//비디오 포맷
		pCombo	= (CKTDGUIComboBox*)m_pCamUserWindow->GetControl( L"Cam_User_VideoFormat_List");
		if(pCombo!=NULL)
		{
			pCombo->AddItem(L"MPEG",(LPVOID) NULL, false);
			pCombo->AddItem(L"MJPG",(LPVOID) NULL, false);
			pCombo->AddItem(L"MP4V",(LPVOID) NULL, false);
			pCombo->AddItem(L"ARGB",(LPVOID) NULL, false);
			pCombo->AddItem(L"RGB24",(LPVOID) NULL, false);
			pCombo->AddItem(L"YV12",(LPVOID) NULL, false);
			if( NULL != g_pMain )
			{	//새로 생성 되어도 기존 값 유지 할 수 있도록 수정.
				pCombo->SetSelectedByIndex( g_pMain->GetGameOption().GetVideoFormatIndex() ) ;
			}
		}
	}
	
	if( m_pCamUserWindow != NULL )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pCamUserWindow, XDL_POP_UP);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pCamUserWindow, true );
		m_pCamUserWindow->SetShowEnable(bShow, bShow);
	}
}
//void CX2StateOption::IMEDataGetUserVideoOptionSetting()
//{
//	WCHAR wszText[64] = {0,};
//
//	//문자열 예외처리를 해야지 정상 작동을 보장할수 있다
//	//한글 이나 다른 문자가 들어가면 예외처리 일단 패스 완성본에 넣자
//	CKTDGUIIMEEditBox * pIME=(CKTDGUIIMEEditBox *)m_pDLGOptionWindow->GetControl( L"Cam_IME1" );
//	g_pMain->GetGameOption().SetVideoSizeW(_wtoi(pIME->GetText()));
//	
//
//	pIME=(CKTDGUIIMEEditBox *)m_pDLGOptionWindow->GetControl( L"Cam_IME2" );
//	g_pMain->GetGameOption().SetVideoSizeH(_wtoi(pIME->GetText()));
//	
//	//g_pMain->GetGameOption().SetUserSetting(true);
//
//}
#endif BANDICAM_RECORDING


void CX2StateOption::ShowCommunityOption( bool bShow )
{
#if 0
	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Static_Option_Window_Community" );


	CKTDGUIRadioButton* pRadio_DENY_FRIENDSHIP_ON					= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_FRIENDSHIP_ON" );
	CKTDGUIRadioButton* pRadio_DENY_FRIENDSHIP_OFF					= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_FRIENDSHIP_OFF" );
	CKTDGUIRadioButton* pRadio_DENY_WHISPER_ON						= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_WHISPER_ON" );
	CKTDGUIRadioButton* pRadio_DENY_WHISPER_OFF						= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_WHISPER_OFF" );
	CKTDGUIRadioButton* pRadio_DENY_WHISPER_ONLY_FRIENDS			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_WHISPER_ONLY_FRIENDS" );
	CKTDGUIRadioButton* pRadio_DENY_PARTY_ON						= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PARTY_ON" );
	CKTDGUIRadioButton* pRadio_DENY_PARTY_OFF						= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PARTY_OFF" );
	CKTDGUIRadioButton* pRadio_DENY_PARTY_ONLY_FRIENDS				= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PARTY_ONLY_FRIENDS" );
	CKTDGUIRadioButton* pRadio_DENY_PERSONAL_TRADE_ON				= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PERSONAL_TRADE_ON" );
	CKTDGUIRadioButton* pRadio_DENY_PERSONAL_TRADE_OFF				= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PERSONAL_TRADE_OFF" );
	CKTDGUIRadioButton* pRadio_DENY_PERSONAL_TRADE_ONLY_FRIENDS		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PERSONAL_TRADE_ONLY_FRIENDS" );


	//CKTDGUIButton* pButton_BlackList = (CKTDGUIButton*) m_pDLGOptionWindow->GetControl( L"Blacklist" );


	pStatic->SetShowEnable( bShow, bShow );				

	pRadio_DENY_FRIENDSHIP_ON->SetShowEnable( bShow, bShow );				
	pRadio_DENY_FRIENDSHIP_OFF->SetShowEnable( bShow, bShow );								

	pRadio_DENY_WHISPER_ON->SetShowEnable( bShow, bShow );									
	pRadio_DENY_WHISPER_OFF->SetShowEnable( bShow, bShow );									
	pRadio_DENY_WHISPER_ONLY_FRIENDS->SetShowEnable( bShow, bShow );						

	pRadio_DENY_PARTY_ON->SetShowEnable( bShow, bShow );									
	pRadio_DENY_PARTY_OFF->SetShowEnable( bShow, bShow );									
	pRadio_DENY_PARTY_ONLY_FRIENDS->SetShowEnable( bShow, bShow );							

	pRadio_DENY_PERSONAL_TRADE_ON->SetShowEnable( bShow, bShow );							
	pRadio_DENY_PERSONAL_TRADE_OFF->SetShowEnable( bShow, bShow );							
	pRadio_DENY_PERSONAL_TRADE_ONLY_FRIENDS->SetShowEnable( bShow, bShow );					


	//pButton_BlackList->SetShowEnable( bShow, bShow );
#endif
}
#ifdef SERV_SECOND_SECURITY
void CX2StateOption::ShowStaticSecurity( bool bShow )
{
	return;
}
#endif SERV_SECOND_SECURITY


void CX2StateOption::InitGraphicOption()
{
	CX2GameOption::OptionList* pOptionList = &g_pMain->GetGameOption().GetOptionList();

	CKTDGUIButton* pButton_Grahpic_AutoSetLevel_Left		= (CKTDGUIButton*) m_pDLGOptionWindow->GetControl( L"ButtonLeft_AutoSetLevel" );
	CKTDGUIButton* pButton_Grahpic_AutoSetLevel_Right		= (CKTDGUIButton*) m_pDLGOptionWindow->GetControl( L"ButtonRight_AutoSetLevel" );

	CKTDGUIStatic* pStatic_Graphic = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Option_Window_Graphic_Num" );
	if ( pStatic_Graphic != NULL )
	{
		WCHAR buf[8];
		StringCchPrintf( buf, ARRAY_SIZE(buf), L"%d", pOptionList->m_iAutoSetLevel );
		pStatic_Graphic->GetString(0)->msg = buf;	

		/*CKTDGUIControl::UIStringData* pString_AutoSetLevel = pStatic_Graphic->GetString(0);
		if ( pString_AutoSetLevel != NULL )
		{
			WCHAR buf[8];
			StringCchPrintf( buf, sizeof(buf), L"%d", pOptionList->m_iAutoSetLevel );
			pString_AutoSetLevel->msg = buf;	
		}*/
	}
	
	
	CKTDGUIRadioButton* pRadio_Graphic_Unit_High			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Unit_High" );
	CKTDGUIRadioButton* pRadio_Graphic_Unit_Middle			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Unit_Middle" );
	CKTDGUIRadioButton* pRadio_Graphic_Unit_Low				= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Unit_Low" );

	CKTDGUIRadioButton* pRadio_Graphic_Texture_High			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Texture_High" );
	CKTDGUIRadioButton* pRadio_Graphic_Texture_Middle		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Texture_Middle" );
	CKTDGUIRadioButton* pRadio_Graphic_Texture_Low			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Texture_Low" );

	CKTDGUIRadioButton* pRadio_Graphic_Background_High		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_BackGround_High" );
	CKTDGUIRadioButton* pRadio_Graphic_Background_Middle	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_BackGround_Middle" );
	CKTDGUIRadioButton* pRadio_Graphic_Background_Low		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_BackGround_Low" );

	CKTDGUIRadioButton* pRadio_Graphic_Effect_High			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Effect_High" );
	CKTDGUIRadioButton* pRadio_Graphic_Effect_Middle		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Effect_Middle" );
	CKTDGUIRadioButton* pRadio_Graphic_Effect_Low			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Effect_Low" );

	CKTDGUIComboBox* pCombo_Graphic_Resoultion				= (CKTDGUIComboBox*) m_pDLGOptionWindow->GetControl( L"Option_Resolution" );
	
	CKTDGUIRadioButton* pRadio_Graphic_Field_Low			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldLow" );
	CKTDGUIRadioButton* pRadio_Graphic_Field_Middle			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldMiddle" );
	CKTDGUIRadioButton* pRadio_Graphic_Field_High			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"FieldHigh" );
	
	CKTDGUICheckBox* pCheckBox_Graphic_FieldParty			= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"FieldParty" );
	CKTDGUICheckBox* pCheckBox_Graphic_FieldSD			= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"FieldSD" );

	
	switch(pOptionList->m_FieldDetail)
	{
	case CX2GameOption::FL_HIGH:		
		pRadio_Graphic_Field_Low->SetChecked(false);
		pRadio_Graphic_Field_Middle->SetChecked(false);
		pRadio_Graphic_Field_High->SetChecked(true);
		break;
	case CX2GameOption::FL_MEDIUM:		
		pRadio_Graphic_Field_Low->SetChecked(false);
		pRadio_Graphic_Field_Middle->SetChecked(true);
		pRadio_Graphic_Field_High->SetChecked(false);
		break;
	case CX2GameOption::FL_LOW:		
		pRadio_Graphic_Field_Low->SetChecked(true);
		pRadio_Graphic_Field_Middle->SetChecked(false);
		pRadio_Graphic_Field_High->SetChecked(false);
		break;
	}

	if(pOptionList->m_bParty == true)
	{
		//pRadio_Graphic_Field_Low->SetEnable(false);
		//pRadio_Graphic_Field_Middle->SetEnable(false);
		//pRadio_Graphic_Field_High->SetEnable(false);
		pCheckBox_Graphic_FieldParty->SetChecked(true);
	}
	else
	{
		//pRadio_Graphic_Field_Low->SetEnable(true);
		//pRadio_Graphic_Field_Middle->SetEnable(true);
		//pRadio_Graphic_Field_High->SetEnable(true);
		pCheckBox_Graphic_FieldParty->SetChecked(false);
	}
	if(pOptionList->m_bSD == true)
	{
		pCheckBox_Graphic_FieldSD->SetChecked(true);
	}
	else
	{
		pCheckBox_Graphic_FieldSD->SetChecked(false);
	}
	

	if( pOptionList->m_UnitDetail == CX2GameOption::OL_HIGH )
	{
		pRadio_Graphic_Unit_High->SetChecked( true );
		pRadio_Graphic_Unit_Middle->SetChecked( false );
		pRadio_Graphic_Unit_Low->SetChecked( false );
	}
	else if( pOptionList->m_UnitDetail == CX2GameOption::OL_MEDIUM )
	{
		pRadio_Graphic_Unit_High->SetChecked( false );
		pRadio_Graphic_Unit_Middle->SetChecked( true );
		pRadio_Graphic_Unit_Low->SetChecked( false );
	}
	else
	{
		pRadio_Graphic_Unit_High->SetChecked( false );
		pRadio_Graphic_Unit_Middle->SetChecked( false );
		pRadio_Graphic_Unit_Low->SetChecked( true );
	}

	if( pOptionList->m_TexDetail == CX2GameOption::OL_HIGH )
	{
		pRadio_Graphic_Texture_High->SetChecked( true );
		pRadio_Graphic_Texture_Middle->SetChecked( false );
		pRadio_Graphic_Texture_Low->SetChecked( false );
	}
	else if( pOptionList->m_TexDetail == CX2GameOption::OL_MEDIUM )
	{
		pRadio_Graphic_Texture_High->SetChecked( false );
		pRadio_Graphic_Texture_Middle->SetChecked( true );
		pRadio_Graphic_Texture_Low->SetChecked( false );
	}
	else
	{
		pRadio_Graphic_Texture_High->SetChecked( false );
		pRadio_Graphic_Texture_Middle->SetChecked( false );
		pRadio_Graphic_Texture_Low->SetChecked( true );
	}



	if( pOptionList->m_MapDetail == CX2GameOption::OL_HIGH )
	{
		pRadio_Graphic_Background_High->SetChecked( true );
		pRadio_Graphic_Background_Middle->SetChecked( false );
		pRadio_Graphic_Background_Low->SetChecked( false );
	}
	else if( pOptionList->m_MapDetail == CX2GameOption::OL_MEDIUM )
	{
		pRadio_Graphic_Background_High->SetChecked( false );
		pRadio_Graphic_Background_Middle->SetChecked( true );
		pRadio_Graphic_Background_Low->SetChecked( false );
	}
	else
	{
		pRadio_Graphic_Background_High->SetChecked( false );
		pRadio_Graphic_Background_Middle->SetChecked( false );
		pRadio_Graphic_Background_Low->SetChecked( true );
	}


#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( pOptionList->m_eEffect == CX2GameOption::OL_HIGH )
	{
		pRadio_Graphic_Effect_High->SetChecked( true );
		pRadio_Graphic_Effect_Middle->SetChecked( false );
		pRadio_Graphic_Effect_Low->SetChecked( false );
	}
	else if( pOptionList->m_eEffect == CX2GameOption::OL_MEDIUM )
	{
		pRadio_Graphic_Effect_High->SetChecked( false );
		pRadio_Graphic_Effect_Middle->SetChecked( true );
		pRadio_Graphic_Effect_Low->SetChecked( false );
	}
	else
	{
		pRadio_Graphic_Effect_High->SetChecked( false );
		pRadio_Graphic_Effect_Middle->SetChecked( false );
		pRadio_Graphic_Effect_Low->SetChecked( true );
	}


#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( pOptionList->m_bEffect == true )
	{
		pRadio_Graphic_Effect_High->SetChecked( true );
		pRadio_Graphic_Effect_Middle->SetChecked( false );
		pRadio_Graphic_Effect_Low->SetChecked( false );
	}
	else
	{
		pRadio_Graphic_Effect_High->SetChecked( false );
		pRadio_Graphic_Effect_Middle->SetChecked( false );
		pRadio_Graphic_Effect_Low->SetChecked( true );
	}
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION


	//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
	// 기타 옵션에서 옮겨옴
	CKTDGUICheckBox* pCheckWindowMode = (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Window_Mode" );
	if( NULL != pCheckWindowMode )
	{
		pCheckWindowMode->SetChecked( !pOptionList->m_bFullScreen );
	}
#endif	CHAT_WINDOW_IMPROV
	//}} kimhc // 2010.3.12 //	채팅창 개편

#ifdef VERTICAL_SYNC_OPTION
	CKTDGUICheckBox* pCheckFlashEffect = (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"FlashEffect" );
	if ( NULL != pCheckFlashEffect )
	{
		pCheckFlashEffect->SetChecked( pOptionList->m_bFlashEffect );
	}
#endif
}


void CX2StateOption::InitSoundOption()
{
	CX2GameOption::OptionList* pOptionList = &g_pMain->GetGameOption().GetOptionList();

//	CKTDGUIRadioButton* pRadio_Sound_On		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Sound_On" );
	//CKTDGUIRadioButton* pRadio_Sound_Off	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Sound_Off" );

//	CKTDGUIRadioButton* pRadio_BGM_On		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_BGM_On" );
//	CKTDGUIRadioButton* pRadio_BGM_Off		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_BGM_Off" );

	CKTDGUICheckBox* pCheckBoxSoundOff		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"NoSound" );
	CKTDGUICheckBox* pCheckBoxEffectSoundOff = (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"NoEffect_Sound" );



	CKTDGUIRadioButton* pRadioEnable3DSoundOn	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"3DSound_on" );
	CKTDGUIRadioButton* pRadioEnable3DSoundOff	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"3DSound_off" );
	if( NULL != pRadioEnable3DSoundOff )
	{
		pRadioEnable3DSoundOff->SetChecked( false );
	}
	if( NULL != pRadioEnable3DSoundOn )
	{
		pRadioEnable3DSoundOn->SetChecked( false );
	}


	

	CKTDGUISlider* pSlider_Sound_Volume		= (CKTDGUISlider*) m_pDLGOptionWindow->GetControl( L"Slider_Sound_Volume" );
	CKTDGUISlider* pSlider_BGM_Volume		= (CKTDGUISlider*) m_pDLGOptionWindow->GetControl( L"Slider_BGM_Volume" );

//	pRadio_Sound_On->SetChecked( pOptionList->m_bSound );		
	//pRadio_Sound_Off->SetChecked( !pOptionList->m_bSound );	

//	pRadio_BGM_On->SetChecked( pOptionList->m_bMusic );		
//	pRadio_BGM_Off->SetChecked( !pOptionList->m_bMusic );

	pCheckBoxSoundOff->SetChecked( !pOptionList->m_bMusic );
	pCheckBoxEffectSoundOff->SetChecked(  !pOptionList->m_bSound );

	pSlider_Sound_Volume->SetValue( (int) pOptionList->m_fSoundVolume );
	pSlider_BGM_Volume->SetValue( (int) pOptionList->m_fMusicVolume );

}


void CX2StateOption::InitOtherOption()
{
	CX2GameOption::OptionList* pOptionList = &g_pMain->GetGameOption().GetOptionList();

	CKTDGUIRadioButton* pRadio_Dynamic_Camera_On	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Dynamic_Camera_On" );
	CKTDGUIRadioButton* pRadio_Dynamic_Camera_Off	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Dynamic_Camera_Off" );

	CKTDGUIRadioButton* pRadio_Play_Guide_On		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Play_Guide_On" );
	CKTDGUIRadioButton* pRadio_Play_Guide_Off		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Window_Play_Guide_Off" );

	pRadio_Dynamic_Camera_On->SetChecked( pOptionList->m_bDynamicCamera );
	pRadio_Dynamic_Camera_Off->SetChecked( !pOptionList->m_bDynamicCamera );

	pRadio_Play_Guide_On->SetChecked( pOptionList->m_bPlayGuide );
	pRadio_Play_Guide_Off->SetChecked( !pOptionList->m_bPlayGuide );

#ifdef VERTICAL_SYNC_OPTION
	CKTDGUIRadioButton* pRadio_Vertical_Sync_On		= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Vertical_Sync_On" );
	CKTDGUIRadioButton* pRadio_Vertical_Sync_Off	= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"Option_Vertical_Sync_Off" );

	pRadio_Vertical_Sync_On->SetChecked( pOptionList->m_bVerticalSync );
	pRadio_Vertical_Sync_Off->SetChecked( !pOptionList->m_bVerticalSync );
#endif

	switch( pOptionList->m_eSkillCutinShowState )
	{
	case CX2GameOption::SCSS_ONESELF:
		{
			CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*> (m_pDLGOptionWindow->GetControl( L"Option_SkillCutin_OneSelf" ));
			pRadio->SetChecked(true);
		} break;
	case CX2GameOption::SCSS_PARTY_MEMBER:
		{
			CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*> (m_pDLGOptionWindow->GetControl( L"Option_SkillCutin_PartyMember" ));
			pRadio->SetChecked(true);
		} break;
	case CX2GameOption::SCSS_HIDE:
		{
			CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*> (m_pDLGOptionWindow->GetControl( L"Option_SkillCutin_Hide" ));
			pRadio->SetChecked(true);
		} break;
	}
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGOptionWindow->GetControl(L"SkillCutin"));
	pStatic->GetString(0)->msg = GET_STRING(STR_ID_20328);
	pStatic->GetString(1)->msg = GET_STRING(STR_ID_20329);
	pStatic->GetString(2)->msg = GET_STRING(STR_ID_20330);
	pStatic->GetString(3)->msg = GET_STRING(STR_ID_20331);

#ifdef SKILL_SLOT_UI_TYPE_B
	if( true == pOptionList->m_bIsSkillUITypeA )
	{
		CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*> (m_pDLGOptionWindow->GetControl( L"Option_SkillUI_TypeA" ));
		if( NULL != pRadio )
			pRadio->SetChecked(true);
	}
	else
	{
		CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*> (m_pDLGOptionWindow->GetControl( L"Option_SkillUI_TypeB" ));
		if( NULL != pRadio )
			pRadio->SetChecked(true);
	}
#endif //SKILL_SLOT_UI_TYPE_B
	
	CKTDGUICheckBox* pCheckBoxLevel		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Level" );
	CKTDGUICheckBox* pCheckBoxPvpRank	= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"PVP" );
	//CKTDGUICheckBox* pCheckBoxGuild		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"CheckBox_Guild" );
	CKTDGUICheckBox* pCheckBoxNothing	= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Nothing" );
#ifdef SERV_LOCAL_RANKING_SYSTEM
	CKTDGUICheckBox* pCheckBoxRankUpShow = static_cast<CKTDGUICheckBox*>( m_pDLGOptionWindow->GetControl( L"DungeonRank" ) );
	if( NULL != pCheckBoxRankUpShow )
		pCheckBoxRankUpShow->SetChecked( pOptionList->m_bShowRankUpInDungeon );
#endif //SERV_LOCAL_RANKING_SYSTEM
	pCheckBoxLevel->SetChecked( pOptionList->m_bLevel );
	pCheckBoxPvpRank->SetChecked( pOptionList->m_bPvpRank );
	//pCheckBoxGuild->SetChecked( pOptionList->m_bGuild );
	pCheckBoxNothing->SetChecked( !pOptionList->m_bNothing );

	CKTDGUICheckBox* pCheckRefuseParty		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"party_veto" );
	CKTDGUICheckBox* pCheckRefuseFriend		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"friend_veto" );
	CKTDGUICheckBox* pCheckRefuseTrade		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Trade_veto" );

	pCheckRefuseParty->SetChecked( pOptionList->m_bRefuseParty );
	pCheckRefuseFriend->SetChecked( pOptionList->m_bRefuseFriend );
	pCheckRefuseTrade->SetChecked( pOptionList->m_bRefusePersonalTrade );

	//{{ kimhc // 2009-10-09 // 길드초대 거부 추가
#ifdef	GUILD_MANAGEMENT
	CKTDGUICheckBox* pCheckRefuseGuildInvitation		= NULL;
	pCheckRefuseGuildInvitation = static_cast< CKTDGUICheckBox* >( m_pDLGOptionWindow->GetControl( L"guild_veto" ) );

	if ( pCheckRefuseGuildInvitation != NULL )
		pCheckRefuseGuildInvitation->SetChecked( pOptionList->m_bRefuseGuildInvitation );
#else	GUILD_MANAGEMENT
	CKTDGUICheckBox* pCheckRefuseGuildInvitation		= NULL;
	pCheckRefuseGuildInvitation = static_cast< CKTDGUICheckBox* >( m_pDLGOptionWindow->GetControl( L"guild_veto" ) );

	if ( pCheckRefuseGuildInvitation != NULL )
		pCheckRefuseGuildInvitation->SetShowEnable( false, false );

	CKTDGUIStatic*	pStaticRefuseBuildInvitation		= NULL;
	pStaticRefuseBuildInvitation = static_cast< CKTDGUIStatic* >( m_pDLGOptionWindow->GetControl( L"StaticGuildVeto" ) );

	if ( pStaticRefuseBuildInvitation != NULL )
		pStaticRefuseBuildInvitation->SetShowEnable( false, false );
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-10-09 // 길드초대 거부 추가


#ifdef ADDED_RELATIONSHIP_SYSTEM		/// 커플신청 거부 추가
	CKTDGUICheckBox* pCheckRefuseCouple		= NULL;
	pCheckRefuseCouple = static_cast< CKTDGUICheckBox* >( m_pDLGOptionWindow->GetControl( L"couple_veto" ) );

	if ( pCheckRefuseCouple != NULL )
		pCheckRefuseCouple->SetChecked( pOptionList->m_bRefuseRequestCouple );
#endif // ADDED_RELATIONSHIP_SYSTEM


#ifdef FIX_INVITE_PVP_PLAYER // 김태환		/// 대전 초대 거부 추가
	CKTDGUICheckBox* pCheckInvitePVPVeto	= NULL;
	pCheckInvitePVPVeto						= static_cast< CKTDGUICheckBox* >( m_pDLGOptionWindow->GetControl( L"Fight_veto" ) );

	if ( pCheckInvitePVPVeto != NULL )
		pCheckInvitePVPVeto->SetChecked( pOptionList->m_bRefuseInvitePVP );
#endif // FIX_INVITE_PVP_PLAYER


	//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifndef	CHAT_WINDOW_IMPROV
	CKTDGUICheckBox* pCheckWindowMode = (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Window_Mode" );
	if( NULL != pCheckWindowMode )
	{
		pCheckWindowMode->SetChecked( !pOptionList->m_bFullScreen );
	}
#endif	CHAT_WINDOW_IMPROV
	//}} kimhc // 2010.3.12 //	채팅창 개편
}

void CX2StateOption::InitCommunityOption()
{
	CX2GameOption::OptionList* pOptionList = &g_pMain->GetGameOption().GetOptionList();


	CKTDGUIRadioButton* pRadio_DENY_FRIENDSHIP_ON						= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_FRIENDSHIP_ON" );
	CKTDGUIRadioButton* pRadio_DENY_FRIENDSHIP_OFF						= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_FRIENDSHIP_OFF" );

	CKTDGUIRadioButton* pRadio_DENY_WHISPER_ON							= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_WHISPER_ON" );
	CKTDGUIRadioButton* pRadio_DENY_WHISPER_OFF							= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_WHISPER_OFF" );
	CKTDGUIRadioButton* pRadio_DENY_WHISPER_ONLY_FRIENDS				= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_WHISPER_ONLY_FRIENDS" );

	CKTDGUIRadioButton* pRadio_DENY_PARTY_ON							= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PARTY_ON" );
	CKTDGUIRadioButton* pRadio_DENY_PARTY_OFF							= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PARTY_OFF" );
	CKTDGUIRadioButton* pRadio_DENY_PARTY_ONLY_FRIENDS					= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PARTY_ONLY_FRIENDS" );

	CKTDGUIRadioButton* pRadio_DENY_PERSONAL_TRADE_ON					= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PERSONAL_TRADE_ON" );
	CKTDGUIRadioButton* pRadio_DENY_PERSONAL_TRADE_OFF					= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PERSONAL_TRADE_OFF" );
	CKTDGUIRadioButton* pRadio_DENY_PERSONAL_TRADE_ONLY_FRIENDS			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"SOUCM_OPTION_COMMUNITY_DENY_PERSONAL_TRADE_ONLY_FRIENDS" );


	switch( pOptionList->m_eDenyFriendship )
	{
	case CX2GameOption::BL_ON:
		{
			pRadio_DENY_FRIENDSHIP_ON->SetChecked( true );
			pRadio_DENY_FRIENDSHIP_OFF->SetChecked( false );
		} break;

	case CX2GameOption::BL_OFF:
	case CX2GameOption::BL_ONLY_FRIEND:
		{
			pRadio_DENY_FRIENDSHIP_ON->SetChecked( false );
			pRadio_DENY_FRIENDSHIP_OFF->SetChecked( true );
		} break;
	}


	/*switch( pOptionList->m_eDenyInviteGuild )
	{
	case CX2GameOption::BL_ON:
		{
			pRadio_DENY_WHISPER_ON->SetChecked( true );
			pRadio_DENY_WHISPER_OFF->SetChecked( false );
			pRadio_DENY_WHISPER_ONLY_FRIENDS->SetChecked( false );
		} break;

	case CX2GameOption::BL_OFF:
		{
			pRadio_DENY_WHISPER_ON->SetChecked( false );
			pRadio_DENY_WHISPER_OFF->SetChecked( true );
			pRadio_DENY_WHISPER_ONLY_FRIENDS->SetChecked( false );

		} break;

	case CX2GameOption::BL_ONLY_FRIEND:
		{
			pRadio_DENY_WHISPER_ON->SetChecked( false );
			pRadio_DENY_WHISPER_OFF->SetChecked( false );
			pRadio_DENY_WHISPER_ONLY_FRIENDS->SetChecked( true );

		} break;

	}*/



	switch( pOptionList->m_eDenyParty )
	{
	case CX2GameOption::BL_ON:
		{
			pRadio_DENY_PARTY_ON->SetChecked( true );
			pRadio_DENY_PARTY_OFF->SetChecked( false );
			pRadio_DENY_PARTY_ONLY_FRIENDS->SetChecked( false );
		} break;

	case CX2GameOption::BL_OFF:
		{
			pRadio_DENY_PARTY_ON->SetChecked( false );
			pRadio_DENY_PARTY_OFF->SetChecked( true );
			pRadio_DENY_PARTY_ONLY_FRIENDS->SetChecked( false );

		} break;

	case CX2GameOption::BL_ONLY_FRIEND:
		{
			pRadio_DENY_PARTY_ON->SetChecked( false );
			pRadio_DENY_PARTY_OFF->SetChecked( false );
			pRadio_DENY_PARTY_ONLY_FRIENDS->SetChecked( true );

		} break;

	}



	switch( pOptionList->m_eDenyPersonalTrade )
	{
	case CX2GameOption::BL_ON:
		{
			pRadio_DENY_PERSONAL_TRADE_ON->SetChecked( true );
			pRadio_DENY_PERSONAL_TRADE_OFF->SetChecked( false );
			pRadio_DENY_PERSONAL_TRADE_ONLY_FRIENDS->SetChecked( false );
		} break;

	case CX2GameOption::BL_OFF:
		{
			pRadio_DENY_PERSONAL_TRADE_ON->SetChecked( false );
			pRadio_DENY_PERSONAL_TRADE_OFF->SetChecked( true );
			pRadio_DENY_PERSONAL_TRADE_ONLY_FRIENDS->SetChecked( false );

		} break;

	case CX2GameOption::BL_ONLY_FRIEND:
		{
			pRadio_DENY_PERSONAL_TRADE_ON->SetChecked( false );
			pRadio_DENY_PERSONAL_TRADE_OFF->SetChecked( false );
			pRadio_DENY_PERSONAL_TRADE_ONLY_FRIENDS->SetChecked( true );

		} break;
	}
}



void CX2StateOption::AddBlackListWindow( wstring wstrNickName )
{
	if( NULL == m_pDLGBlackList )
		return;

	CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGBlackList->GetControl( L"ListBoxChatContent" );
	if( NULL == pListBox )
		return;

	pListBox->AddItem( wstrNickName.c_str(), NULL );
}

void CX2StateOption::RemoveBlackListWindow( wstring wstrNickName )
{
	if( NULL == m_pDLGBlackList )
		return;

	CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGBlackList->GetControl( L"ListBoxChatContent" );
	if( NULL == pListBox )
		return;

	pListBox->RemoveItemByText( (WCHAR*)wstrNickName.c_str() );
}

void CX2StateOption::UpdateBlackListWindow()
{
	if( NULL == m_pDLGBlackList )
		return;

	CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGBlackList->GetControl( L"ListBoxChatContent" );
	if( NULL == pListBox )
		return;

	pListBox->RemoveAllItems();

	CX2GameOption::OptionList* pOptionList = &g_pMain->GetGameOption().GetOptionList();

	std::map< UidType, wstring >::iterator it;
	for( it=pOptionList->m_mapBlackList.begin(); it!= pOptionList->m_mapBlackList.end() ; it++ )
	{
		wstring& wstrNickName = it->second;
		pListBox->AddItem( wstrNickName.c_str(), NULL );
	}
}

void CX2StateOption::OpenBlackListWindow()
{
	if( NULL == m_pDLGBlackList )
		return;

	m_pDLGBlackList->SetShowEnable( true, true );
}

void CX2StateOption::CloseBlackListWindow()
{
	CloseBlackListAddWindow();

	if( NULL == m_pDLGBlackList )
		return;

	m_pDLGBlackList->SetShowEnable( false, false );
}

void CX2StateOption::OpenBlackListAddWindow()
{
	if( NULL == m_pDLGBlackListAdd )
		return;

	m_pDLGBlackListAdd->SetShowEnable( true, true );

}

void CX2StateOption::CloseBlackListAddWindow()
{
	if( NULL == m_pDLGBlackListAdd )
		return;

	m_pDLGBlackListAdd->SetShowEnable( false, false );
}


bool CX2StateOption::Handler_EGS_UPDATE_COMMUNITY_OPTION_REQ()
{
	CX2GameOption::OptionList* pOptionList = &g_pMain->GetGameOption().GetOptionList();

	KEGS_UPDATE_COMMUNITY_OPTION_REQ kPacket;
	
	kPacket.m_kDenyOptions.m_cDenyInviteGuild	= (char) pOptionList->m_eDenyInviteGuild;
	kPacket.m_kDenyOptions.m_cDenyPersonalTrade	= (char) pOptionList->m_eDenyPersonalTrade;
	//kPacket.m_kDenyOptions.m_cDenyFriendShip	= (char) pOptionList->m_eDenyFriendship;
	//kPacket.m_kDenyOptions.m_cDenyParty			= (char) pOptionList->m_eDenyParty;
	if( pOptionList->m_bRefuseParty == true )
		kPacket.m_kDenyOptions.m_cDenyParty = CX2GameOption::BL_ON;
	else
		kPacket.m_kDenyOptions.m_cDenyParty = CX2GameOption::BL_OFF;

	if( pOptionList->m_bRefuseFriend == true )
		kPacket.m_kDenyOptions.m_cDenyFriendShip = CX2GameOption::BL_ON;
	else
		kPacket.m_kDenyOptions.m_cDenyFriendShip = CX2GameOption::BL_OFF;

	if( pOptionList->m_bRefusePersonalTrade == true )
		kPacket.m_kDenyOptions.m_cDenyPersonalTrade = CX2GameOption::BL_ON;
	else
		kPacket.m_kDenyOptions.m_cDenyPersonalTrade = CX2GameOption::BL_OFF;

	//{{ kimhc // 2009-10-09 // 길드초대 거부 추가
#ifdef	GUILD_MANAGEMENT
	if ( pOptionList->m_bRefuseGuildInvitation == true )
		kPacket.m_kDenyOptions.m_cDenyInviteGuild = CX2GameOption::BL_ON;
	else
		kPacket.m_kDenyOptions.m_cDenyInviteGuild = CX2GameOption::BL_OFF;
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-10-09 // 길드초대 거부 추가

#ifdef ADDED_RELATIONSHIP_SYSTEM	/// 커플신청 거부 추가
	if ( pOptionList->m_bRefuseRequestCouple == true )
		kPacket.m_kDenyOptions.m_cDenyRequestCouple = CX2GameOption::BL_ON;
	else
		kPacket.m_kDenyOptions.m_cDenyRequestCouple = CX2GameOption::BL_OFF;
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef FIX_INVITE_PVP_PLAYER // 김태환	/// 대전 초대 거부 추가
	if ( pOptionList->m_bRefuseInvitePVP == true )
		kPacket.m_kDenyOptions.m_cDenyInvitePracticePVP = CX2GameOption::BL_ON;
	else
		kPacket.m_kDenyOptions.m_cDenyInvitePracticePVP = CX2GameOption::BL_OFF;
#endif // FIX_INVITE_PVP_PLAYER

	g_pData->GetServerProtocol()->SendPacket( EGS_UPDATE_COMMUNITY_OPTION_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_UPDATE_COMMUNITY_OPTION_ACK, 60.0f );

	return true;
}

bool CX2StateOption::Handler_EGS_UPDATE_COMMUNITY_OPTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_COMMUNITY_OPTION_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_UPDATE_COMMUNITY_OPTION_ACK ) == true )
	{		
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			CX2GameOption::OptionList* pOptionList = &g_pMain->GetGameOption().GetOptionList();

			pOptionList->m_eDenyFriendship      = (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyFriendShip;
			pOptionList->m_eDenyInviteGuild		= (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyInviteGuild;	
			pOptionList->m_eDenyParty			= (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyParty;
			pOptionList->m_eDenyPersonalTrade   = (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyPersonalTrade;

			g_pMain->GetGameOption().SetRefuseParty( pOptionList->m_eDenyParty );
			g_pMain->GetGameOption().SetRefuseFriend( pOptionList->m_eDenyFriendship );
			g_pMain->GetGameOption().SetRefusePersonalTrade( pOptionList->m_eDenyPersonalTrade );

			CKTDGUICheckBox* pCheckRefuseParty		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"party_veto" );
			CKTDGUICheckBox* pCheckRefuseFriend		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"friend_veto" );
			CKTDGUICheckBox* pCheckRefuseTrade		= (CKTDGUICheckBox*) m_pDLGOptionWindow->GetControl( L"Trade_veto" );

			pCheckRefuseParty->SetChecked( pOptionList->m_bRefuseParty );
			pCheckRefuseFriend->SetChecked( pOptionList->m_bRefuseFriend );
			pCheckRefuseTrade->SetChecked( pOptionList->m_bRefusePersonalTrade );

			//{{ kimhc // 2009-10-09 // 길드초대 거부 추가
#ifdef	GUILD_MANAGEMENT
			CKTDGUICheckBox* pCheckRefuseGuildInvitation		= NULL;
			pCheckRefuseGuildInvitation = static_cast< CKTDGUICheckBox* >( m_pDLGOptionWindow->GetControl( L"guild_veto" ) );

			if ( pCheckRefuseGuildInvitation != NULL )
				pCheckRefuseGuildInvitation->SetChecked( pOptionList->m_bRefuseGuildInvitation );
#endif	GUILD_MANAGEMENT
			//}} kimhc // 2009-10-09 // 길드초대 거부 추가

#ifdef ADDED_RELATIONSHIP_SYSTEM	/// 커플신청 거부 추가
			CKTDGUICheckBox* pCheckRefuseCouple		= NULL;
			pCheckRefuseCouple			= static_cast< CKTDGUICheckBox* >( m_pDLGOptionWindow->GetControl( L"couple_veto" ) );

			if ( pCheckRefuseCouple != NULL )
				pCheckRefuseCouple->SetChecked( pOptionList->m_bRefuseRequestCouple );
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef FIX_INVITE_PVP_PLAYER // 김태환		/// 대전 초대 거부 추가
			CKTDGUICheckBox* pCheckInvitePVPVeto	= NULL;
			pCheckInvitePVPVeto						= static_cast< CKTDGUICheckBox* >( m_pDLGOptionWindow->GetControl( L"Fight_veto" ) );

			if ( pCheckInvitePVPVeto != NULL )
				pCheckInvitePVPVeto->SetChecked( pOptionList->m_bRefuseInvitePVP );
#endif // FIX_INVITE_PVP_PLAYER

			return true;
		}
	}
	return false;
}

void CX2StateOption::ResetResolutionToOriginal()
{
	CX2GameOption* pGameOption = &g_pMain->GetGameOption();
	pGameOption->SetResolution( (int) m_vOriginalResolution.x, (int) m_vOriginalResolution.y );

	if( NULL != m_pDLGOptionWindow /*&&
		true == m_pDLGOptionWindow->GetShow()*/ )
	{
		CKTDGUIComboBox* pCombo_Graphic_Resoultion = (CKTDGUIComboBox*) m_pDLGOptionWindow->GetControl( L"Option_Resolution" );
		if( NULL != pCombo_Graphic_Resoultion )
		{
			WCHAR strResolution[50];
			//{{ 강정훈 / 2010.08.31 / 해상도 변경 취소 시 이전 선택 해상도로 복귀 수정
			StringCchPrintf( strResolution, ARRAY_SIZE(strResolution), L"%d x %d", (int)m_vOriginalResolution.x, (int)m_vOriginalResolution.y );
			//StringCchPrintf( strResolution, ARRAY_SIZE(strResolution), L"%d by %d", (int)m_vOriginalResolution.x, (int)m_vOriginalResolution.y );
			//}} 강정훈 / 2010.08.31 / 해상도 변경 취소 시 이전 선택 해상도로 복귀 수정
			pCombo_Graphic_Resoultion->SetSelectedByText( strResolution, false );
		}
	}
}


//////////////////////////////////////////////////////////////////////////
CX2StateOption::ConfirmDialog::ConfirmDialog( const WCHAR* wszMsgTemplet, float fTimeLeft, int iYesMsg, int iNoMsg, CKTDXStage* pStage )
{
	m_wstrMsgTemplet = wszMsgTemplet;
	m_fTimeLeft = fTimeLeft;

	m_pDLG = NULL;

	WCHAR wszText[512] = L"";
	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%s (%d)", m_wstrMsgTemplet.c_str(), (int)m_fTimeLeft );
    
	m_pDLG = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(300,310), wszText, iYesMsg, pStage, iNoMsg );
}

void CX2StateOption::ConfirmDialog::OnFrameMove( double fTime, float fElapsedTime )
{
	if( m_fTimeLeft <= 0.f )
		return;

	int iOldTimeLeft = (int)m_fTimeLeft;
	m_fTimeLeft -= fElapsedTime;

	if( m_pDLG == NULL || 
		g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLG ) == false )
		return;

	if( m_fTimeLeft < 0.f )
		m_pDLG->SetShowEnable( false, false );

	if( false == m_pDLG->GetShow() )
		return;

	if( iOldTimeLeft != (int)m_fTimeLeft )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLG->GetControl( L"StaticMsgBoxBG" );

		if( pStatic != NULL )
		{
			WCHAR wszText[512] = L"";
			swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%s (%d)", m_wstrMsgTemplet.c_str(), (int)m_fTimeLeft );

			pStatic->GetString(0)->msg = wszText;
		}
	}
}
#ifdef SERV_KEY_MAPPING_INT
void CX2StateOption::ShowKeyConfigDLG( bool bShow )
{	
	if(bShow)
	{
		SAFE_DELETE_DIALOG(m_DLGKeyConfig);

		m_DLGKeyConfig = new CKTDGUIDialog( this, L"DLG_UI_Option_KeyConfig.lua", 0.07f, 0 );
		
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_DLGKeyConfig );
		UpdateKeyConfigList();
	}
	else
	{
		if( m_DLGKeyConfig != NULL )
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_DLGKeyConfig, NULL, false );
	}
}

void CX2StateOption::UpdateKeyConfigList()
{
	if( m_DLGKeyConfig != NULL )
	{
		for( int i = 0; i <= 20; ++i )	// 20은 패드 옵션 설정 키 개수
		{
			WCHAR wszText[256] = L"";
			StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"Joy%d", i );

			CKTDGUIStatic* pJoyconfig	= (CKTDGUIStatic*) m_DLGKeyConfig->GetControl( wszText );
			if(pJoyconfig != NULL)
			{
				GAME_ACTION eGameAction = MappingKeyConfigIndex( i );

				wstring wstrName = L"";
				wstrName += GetJoystickButtonName( g_pKTDXApp->GetDIManager()->GetJoystic()->GetActionKey( eGameAction ) );

				pJoyconfig->SetString(0 ,wstrName.c_str());
			}
		}	
	}
	return;
}

GAME_ACTION CX2StateOption::MappingKeyConfigIndex( int iKeyConfigIndex )
{
	GAME_ACTION eGameAction = GAME_ACTION_NONE;

	switch( iKeyConfigIndex )
	{
	case 0:				// 위로
		{
			eGameAction = GAMEACTION_UP;
		}
		break;
	case 1:				// 아래로
		{
			eGameAction = GAMEACTION_DOWN;
		}
		break;
	case 2:				// 왼쪽
		{
			eGameAction = GAMEACTION_LEFT;
		}
		break;
	case 3:				// 오른쪽
		{
			eGameAction = GAMEACTION_RIGHT;
		}
		break;
	case 4:				// 약공격
		{
			eGameAction = GAMEACTION_ATTACK_FAST;
		}
		break;
	case 5:				// 강공격
		{
			eGameAction = GAMEACTION_ATTACK_STRONG;
		}
		break;
	case 6:				// 스킬슬롯1
		{
			eGameAction = GAMEACTION_SKILL1;
		}
		break;
	case 7:				// 스킬슬롯2
		{
			eGameAction = GAMEACTION_SKILL2;
		}
		break;
	case 8:				// 스킬슬롯3
		{
			eGameAction = GAMEACTION_SKILL3;
		}
		break;
	case 9:				// 스킬슬롯4
		{
			eGameAction = GAMEACTION_SKILL4;
		}
		break;
	case 10:			// 각성
		{
			eGameAction = GAMEACTION_AWAKE;
		}
		break;
	case 11:			// 스킬슬롯체인지
		{
			eGameAction = GAMEACTION_SLOT_CHANGE;
		}
		break;
	case 12:			// 돌아가기
		{
			eGameAction = GAMEACTION_RETURN;
		}
		break;
	case 13:			// 윈도우 닫기
		{
			eGameAction = GAMEACTION_ESCAPE;
		}
		break;
	case 14:			// 레디
		{
			eGameAction = GAMEACTION_PARTYREADY;
		}
		break;
	case 15:			// 퀵슬롯1
		{
			eGameAction = GAMEACTION_QUICKSLOT1;
		}
		break;
	case 16:			// 퀵슬롯2
		{
			eGameAction = GAMEACTION_QUICKSLOT2;
		}
		break;
	case 17:			// 퀵슬롯3
		{
			eGameAction = GAMEACTION_QUICKSLOT3;
		}
		break;
	case 18:			// 퀵슬롯4
		{
			eGameAction = GAMEACTION_QUICKSLOT4;
		}
		break;
	case 19:			// 퀵슬롯5
		{
			eGameAction = GAMEACTION_QUICKSLOT5;
		}
		break;
	case 20:			// 퀵슬롯6
		{
			eGameAction = GAMEACTION_QUICKSLOT6;
		}
		break;
	}

	return eGameAction;
}

void CX2StateOption::SetKeyConfig( GAME_ACTION Action, unsigned char input, InputDevices eType )
{
	switch(eType)
	{
	case JOYSTICK:
		{
			if( g_pKTDXApp->GetDIManager()->GetJoystic() != NULL )
				g_pKTDXApp->GetDIManager()->GetJoystic()->SetAction(Action, input);			
		}
		break;
	default:
		break;
	}
}
wstring CX2StateOption::GetJoystickButtonName( unsigned char input )
{
	switch(input)
	{
	case JB_UP:
		return L"UP";
	case JB_DOWN:
		return L"DOWN";
	case JB_RIGHT:
		return L"RIGHT";
	case JB_LEFT:
		return L"LEFT";
	case JB_LEFT_TRIGGER:
		return L"L-TRIGGER";
	case JB_RIGHT_TRIGGER:
		return L"R-TRIGGER";
	case JB_BUTTON0:
		return L"BUTTON1";
	case JB_BUTTON1:
		return L"BUTTON2";
	case JB_BUTTON2:
		return L"BUTTON3";
	case JB_BUTTON3:
		return L"BUTTON4";
	case JB_BUTTON4:
		return L"BUTTON5";
	case JB_BUTTON5:
		return L"BUTTON6";
	case JB_BUTTON6:
		return L"BUTTON7";
	case JB_BUTTON7:
		return L"BUTTON8";
	case JB_BUTTON8:
		return L"BUTTON9";
	case JB_BUTTON9:
		return L"BUTTON10";
	case JB_BUTTON10:
		return L"BUTTON11";
	case JB_BUTTON11:
		return L"BUTTON12";
	case JB_BUTTON12:
		return L"BUTTON13";
	case JB_BUTTON13:
		return L"BUTTON14";
	case JB_BUTTON14:
		return L"BUTTON15";
	case JB_BUTTON15:
		return L"BUTTON16";
	case JB_BUTTON16:
		return L"BUTTON17";
	case JB_BUTTON17:
		return L"BUTTON18";
	case JB_BUTTON18:
		return L"BUTTON19";
	case JB_BUTTON19:
		return L"BUTTON20";
	case JB_BUTTON20:
		return L"BUTTON21";
	case JB_BUTTON21:
		return L"BUTTON22";
	case JB_BUTTON22:
		return L"BUTTON23";
	case JB_BUTTON23:
		return L"BUTTON24";
	case JB_BUTTON24:
		return L"BUTTON25";
	case JB_BUTTON25:
		return L"BUTTON26";
	case JB_BUTTON26:
		return L"BUTTON27";
	case JB_BUTTON27:
		return L"BUTTON28";
	case JB_BUTTON28:
		return L"BUTTON29";
	case JB_BUTTON29:
		return L"BUTTON30";
	case JB_BUTTON30:
		return L"BUTTON31";
	case JB_BUTTON31:
		return L"BUTTON32";
	default:
		break;
	}
	return L"";
}

void CX2StateOption::ShowInputOption( bool bShow )
{
	CKTDGUIStatic* pStatic_Input = (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Option_Window_Input" );
	pStatic_Input->SetShow( bShow );

	ShowStaticInput(bShow);

	CKTDGUIRadioButton* pRadio_Joy_ON			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"JoyStick_On" );
	CKTDGUIRadioButton* pRadio_Joy_OFF			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"JoyStick_Off" );

	CKTDGUIStatic* pStatic_Joy = NULL;
	pStatic_Joy			= (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Joy_Enable" );
	pStatic_Joy->SetShowEnable(bShow, bShow);
	pStatic_Joy			= (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Joy_On" );
	pStatic_Joy->SetShowEnable(bShow, bShow);
	pStatic_Joy			= (CKTDGUIStatic*) m_pDLGOptionWindow->GetControl( L"Joy_Off" );
	pStatic_Joy->SetShowEnable(bShow, bShow);	

	pRadio_Joy_ON->SetShowEnable(bShow, bShow);
	pRadio_Joy_OFF->SetShowEnable(bShow, bShow);

	CKTDGUIButton* pButtonKeyConfig = NULL;
	pButtonKeyConfig = reinterpret_cast< CKTDGUIButton* >( m_pDLGOptionWindow->GetControl( L"Key_Config" ) );
	pButtonKeyConfig->SetShowEnable(bShow, bShow);
}

void CX2StateOption::ShowStaticInput( bool bShow )
{
	CKTDGUIStatic* pStatic_Input = NULL;
}

void CX2StateOption::InitInputOption()
{
	CKTDGUIRadioButton* pRadio_Joy_ON			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"JoyStick_On" );
	CKTDGUIRadioButton* pRadio_Joy_OFF			= (CKTDGUIRadioButton*) m_pDLGOptionWindow->GetControl( L"JoyStick_Off" );

	pRadio_Joy_ON->SetChecked(g_pKTDXApp->GetDIManager()->GetEnableJoyStick());
	pRadio_Joy_OFF->SetChecked(!g_pKTDXApp->GetDIManager()->GetEnableJoyStick());
}

bool CX2StateOption::Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ()
{
	KEGS_KEYBOARD_MAPPING_INFO_WRITE_REQ  kPacket;

	if( NULL != g_pKTDXApp && NULL != g_pKTDXApp->GetDIManager() )
	{
		g_pKTDXApp->GetDIManager()->GetJoystic()->GetActionKeys( kPacket.m_mapGamePadMappingInfo );

		g_pData->GetServerProtocol()->SendPacket( EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_KEYBOARD_MAPPING_INFO_WRITE_ACK );
	}

	return true;
}
#endif // SERV_KEY_MAPPING_INT