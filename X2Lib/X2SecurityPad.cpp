#include "StdAfx.h"
#include ".\X2SecurityPad.h"

#ifdef SERV_SECOND_SECURITY

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh

namespace _CONST_SECURITY_PAD_
{
	const int MAX_PASSWORD_BUTTON_EXCEPT_RESET_BUTTON = 11;
}
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

CX2SecurityPad::CX2SecurityPad()
{
	m_pDLGSecurityPad	= NULL;
	m_pStaticMemo		= NULL;

	m_bShow				= false;
	m_eState			= SPS_CLOSE;
	m_iPhase			= -1;
	m_bUseSecondPW		= false;

	m_iPasswordIndex	= 0;
	for(int i=0; i<7; i++)
	{
		m_cPassword[i] = 0;
	}

	for(int i=0; i<12; i++)
	{
		m_iButtonSequence[i] = -1;
	}

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	m_wstrAutoAuthorityPassword = L"";
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

#ifdef FIX_REFORM_ENTRY_POINT_2ND // 김종훈, 진입 구조 개편 수정 2차
	m_bIsAutoAuthorityPassworBegin = false;
#endif // FIX_REFORM_ENTRY_POINT_2TH // 김종훈, 진입 구조 개편 수정 2차
}

CX2SecurityPad::~CX2SecurityPad()
{
	SAFE_DELETE_DIALOG(m_pDLGSecurityPad);
}

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
void CX2SecurityPad::SetShow(bool bShow, SECURITY_PAD_STATE eState /* = SPS_CLOSE */, bool bIsDeleteOldDialog /* = true */ )
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
void CX2SecurityPad::SetShow(bool bShow, SECURITY_PAD_STATE eState /* = SPS_CLOSE */ )
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
{
	m_eState = eState;
	m_bShow = bShow;

	if(m_bShow == false || m_eState == SPS_CLOSE)
	{
		m_iPhase = 0;
		ClearInputPassword();

		m_wstrPassword_Temp1.clear();
		m_wstrPassword_Temp2.clear();
		m_pDLGSecurityPad->SetShowEnable(false, false);
		
	}
	else
	{
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		if ( true == bIsDeleteOldDialog )
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
		{
			// Test
			if(m_pDLGSecurityPad != NULL)
			{
				SAFE_DELETE_DIALOG(m_pDLGSecurityPad);
			}
		
			if(m_pDLGSecurityPad == NULL)
			{
	#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
				m_pDLGSecurityPad = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Security_Pad_New.lua", 0.07f, XDL_OPTION );	
	#else  // REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
				m_pDLGSecurityPad = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Security_Pad.lua", 0.07f, XDL_OPTION );	
	#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSecurityPad );
	#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
				m_pStaticMemo = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"g_pStatic_Memo" ));
	#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
			}
		}
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		else
		{	
			if ( m_pDLGSecurityPad == NULL )
			{
				m_pDLGSecurityPad = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Security_Pad_New.lua", 0.07f, XDL_OPTION );	
			}
		}
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSecurityPad );


		{
			for(int i=0; i<_CONST_SECURITY_PAD_::MAX_PASSWORD_BUTTON_EXCEPT_RESET_BUTTON; i++)
			{
				CKTDGUIButton* pButtonDialNumber = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl(GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"ButtonDialNumber" ), i ) ) )  );
				if ( NULL != pButtonDialNumber ) 
				{
					pButtonDialNumber->SetShowEnable( true, true );
				}
			}

			CKTDGUIStatic* pNoticeStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"NoticeStatic" ) );
			if ( NULL != pNoticeStatic )
				pNoticeStatic->SetShow ( false );

			CKTDGUIStatic* pHideNumberPadStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"HideNumberPad" ) );
			if ( NULL != pHideNumberPadStatic )
				pHideNumberPadStatic->SetShow ( false );

			CKTDGUIButton* pChangeButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"ButtonChange" ) );
			if ( NULL != pChangeButton )
				pChangeButton->SetShowEnable( false, false );

			CKTDGUIButton* pReleaseButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"ButtonRelease" ) );
			if ( NULL != pReleaseButton )
				pReleaseButton->SetShowEnable( false, false );

			CKTDGUIButton* pCancleButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"Button_exit2" ) );
			if ( NULL != pCancleButton )
				pCancleButton->SetShowEnable( false, false );

			CKTDGUIButton* pOkButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"ButtonOK" ) );
			if ( NULL != pOkButton )
				pOkButton->SetShowEnable( true, true );

			CKTDGUIButton* pCancelButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"ButtonCancel" ) );
			if ( NULL != pCancelButton )
				pCancelButton->SetShowEnable( true, true );

			CKTDGUIButton* pExitButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"Button_exit" ) );
			if ( NULL != pExitButton )
				pExitButton->SetShowEnable( true, true );

			CKTDGUIStatic* pStaticSignal = static_cast< CKTDGUIStatic* >( m_pDLGSecurityPad->GetControl( L"StaticSignal" )  );
			if ( NULL != pStaticSignal ) 
				pStaticSignal->SetShow ( true );
			
			CKTDGUIStatic* pInputStateStatic = static_cast< CKTDGUIStatic* >( m_pDLGSecurityPad->GetControl( L"InputStateStatic" )  );
			if ( NULL != pInputStateStatic ) 
				pInputStateStatic->SetShow ( true );
		}
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

		m_iPhase = 0;
		m_wstrPassword_Temp1.clear();
		m_wstrPassword_Temp2.clear();
		ClearInputPassword();
		ShuffleNumber();
		m_pDLGSecurityPad->SetShowEnable(true, true);

#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		CKTDGUIStatic* pStaticTip1 = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"g_pStatic_pass_tip1" ));
		CKTDGUIStatic* pStaticTip2 = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"g_pStatic_pass_tip2" ));
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

		switch(m_eState)
		{
		case SPS_GAME_START:
			{
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh

				CKTDGUIStatic* pStaticTitle = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"TitleStatic" ) );
				if ( NULL != pStaticTitle )
				{					
					for ( int idx = 0; idx < pStaticTitle->GetPictureNum(); ++idx )
					{
						if ( NULL != pStaticTitle->GetPicture(idx) )
						{
							pStaticTitle->GetPicture(idx)->SetShow( false );
						}
					}
					if ( NULL != pStaticTitle->GetPicture(1) )
					{
						pStaticTitle->GetPicture(1) ->SetShow ( true );
					}
				}

				CKTDGUIStatic* pInputstateStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"InputStateStatic" ) );
				if ( NULL != pInputstateStatic )
				{					
					for ( int idx = 0; idx < pInputstateStatic->GetPictureNum(); ++idx )
					{
						if ( NULL != pInputstateStatic->GetPicture(idx) )
						{
							pInputstateStatic->GetPicture(idx)->SetShow( false );
						}
					}
					if ( NULL != pInputstateStatic->GetPicture(0) )
						pInputstateStatic->GetPicture(0)->SetShow ( true );
				}
				
				CKTDGUIButton* pExitButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"Button_exit" ) );
				if ( NULL != pExitButton )
					pExitButton->SetShowEnable( true, true );


#else  REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
				m_pStaticMemo->SetString(0, GET_STRING( STR_ID_12537 ));
				pStaticTip1->SetShow(true);
				pStaticTip2->SetShow(false);
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
			}break;
		case SPS_CREATE_PASSWORD:
			{
				m_pDLGSecurityPad->SetCloseCustomUIEventID( SPUM_EXIT );

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
				CKTDGUIStatic* pStaticTitle = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"TitleStatic" ) );
				if ( NULL != pStaticTitle )
				{					
					for ( int idx = 0; idx < pStaticTitle->GetPictureNum(); ++idx )
					{
						if ( NULL != pStaticTitle->GetPicture(idx) )
						{
							pStaticTitle->GetPicture(idx)->SetShow( false );
						}
					}
					if ( NULL != pStaticTitle->GetPicture(0) )
					{
						pStaticTitle->GetPicture(0) ->SetShow ( true );
					}
				}
				CKTDGUIStatic* pInputstateStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"InputStateStatic" ) );
				if ( NULL != pInputstateStatic )
				{					
					for ( int idx = 0; idx < pInputstateStatic->GetPictureNum(); ++idx )
					{
						if ( NULL != pInputstateStatic->GetPicture(idx) )
						{
							pInputstateStatic->GetPicture(idx)->SetShow( false );
						}
					}
					if ( NULL != pInputstateStatic->GetPicture(0) )
						pInputstateStatic->GetPicture(0)->SetShow ( true );
				}
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
				m_pStaticMemo->SetString(0, GET_STRING( STR_ID_12537 ));
				pStaticTip1->SetShow(false);
				pStaticTip2->SetShow(true);
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
			}break;
		case SPS_CHANGE_PASSWORD:
			{

				m_pDLGSecurityPad->SetCloseCustomUIEventID( SPUM_EXIT );
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
				CKTDGUIStatic* pStaticTitle = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"TitleStatic" ) );
				if ( NULL != pStaticTitle )
				{					
					for ( int idx = 0; idx < pStaticTitle->GetPictureNum(); ++idx )
					{
						if ( NULL != pStaticTitle->GetPicture(idx) )
						{
							pStaticTitle->GetPicture(idx)->SetShow( false );
						}
					}
					if ( NULL != pStaticTitle->GetPicture(2) )
					{
						pStaticTitle->GetPicture(2) ->SetShow ( true );
					}
				}
				CKTDGUIStatic* pInputstateStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"InputStateStatic" ) );
				if ( NULL != pInputstateStatic )
				{					
					for ( int idx = 0; idx < pInputstateStatic->GetPictureNum(); ++idx )
					{
						if ( NULL != pInputstateStatic->GetPicture(idx) )
						{
							pInputstateStatic->GetPicture(idx)->SetShow( false );
						}
					}
				
					if ( NULL != pInputstateStatic->GetPicture(2) )
						pInputstateStatic->GetPicture(2)->SetShow ( true );
				}
#else  REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh

				m_pStaticMemo->SetString(0, GET_STRING( STR_ID_12537 ));
				pStaticTip1->SetShow(false);
				pStaticTip2->SetShow(true);
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
			}break;
		case SPS_RELEASE_PASSWORD:
			{				
				m_pDLGSecurityPad->SetCloseCustomUIEventID( SPUM_EXIT );
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
				CKTDGUIStatic* pStaticTitle = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"TitleStatic" ) );
				if ( NULL != pStaticTitle )
				{					
					for ( int idx = 0; idx < pStaticTitle->GetPictureNum(); ++idx )
					{
						if ( NULL != pStaticTitle->GetPicture(idx) )
						{
							pStaticTitle->GetPicture(idx)->SetShow( false );
						}
					}
					if ( NULL != pStaticTitle->GetPicture(3) )
					{
						pStaticTitle->GetPicture(3) ->SetShow ( true );
					}
				}
#else  REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh

				m_pStaticMemo->SetString(0, GET_STRING( STR_ID_12537 ));
				pStaticTip1->SetShow(false);
				pStaticTip2->SetShow(true);
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
			}break;


#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		case SPC_SELECT_STATE :
			{
				CKTDGUIStatic* pHideNumberPadStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"HideNumberPad" ) );
				if ( NULL != pHideNumberPadStatic )
					pHideNumberPadStatic->SetShow ( true );

				m_pDLGSecurityPad->SetCloseCustomUIEventID( SPUM_EXIT );
				CKTDGUIStatic* pStaticTitle = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"TitleStatic" ) );
				if ( NULL != pStaticTitle )
				{					
					for ( int idx = 0; idx < pStaticTitle->GetPictureNum(); ++idx )
					{
						if ( NULL != pStaticTitle->GetPicture(idx) )
						{
							pStaticTitle->GetPicture(idx)->SetShow( false );
						}
					}
				}
				CKTDGUIStatic* pInputstateStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"InputStateStatic" ) );
				if ( NULL != pInputstateStatic )
				{					
					for ( int idx = 0; idx < pInputstateStatic->GetPictureNum(); ++idx )
					{
						if ( NULL != pInputstateStatic->GetPicture(idx) )
						{
							pInputstateStatic->GetPicture(idx)->SetShow( false );
						}
					}
					if ( NULL != pInputstateStatic->GetPicture(0) )
						pInputstateStatic->GetPicture(0)->SetShow ( true );
				}		

				for(int i=0; i<_CONST_SECURITY_PAD_::MAX_PASSWORD_BUTTON_EXCEPT_RESET_BUTTON; i++)
				{
					CKTDGUIButton* pButtonDialNumber = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl(GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"ButtonDialNumber" ), i ) ) )  );
					if ( NULL != pButtonDialNumber ) 
					{
						pButtonDialNumber->SetShowEnable( true, false );
					}
				}
			
				CKTDGUIButton* pChangeButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"ButtonChange" ) );
				if ( NULL != pChangeButton )
					pChangeButton->SetShowEnable( true, true );
				CKTDGUIButton* pReleaseButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"ButtonRelease" ) );
				if ( NULL != pReleaseButton )
					pReleaseButton->SetShowEnable( true, true );
				CKTDGUIButton* pCancleButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"Button_exit2" ) );
				if ( NULL != pCancleButton )
					pCancleButton->SetShowEnable( true, true );

				CKTDGUIStatic* pNoticeStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"NoticeStatic" ) );
				if ( NULL != pNoticeStatic )
					pNoticeStatic->SetShow ( true );
				
				CKTDGUIButton* pOkButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"ButtonOK" ) );
				if ( NULL != pOkButton )
					pOkButton->SetShowEnable( false, false );

				CKTDGUIButton* pExitButton = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"Button_exit" ) );
				if ( NULL != pExitButton )
					pExitButton->SetShowEnable( false, false );

				CKTDGUIStatic* pStaticSignal = static_cast< CKTDGUIStatic* >( m_pDLGSecurityPad->GetControl( L"StaticSignal" )  );
				if ( NULL != pStaticSignal ) 
					pStaticSignal->SetShow ( false );

				CKTDGUIStatic* pInputStateStatic = static_cast< CKTDGUIStatic* >( m_pDLGSecurityPad->GetControl( L"InputStateStatic" )  );
				if ( NULL != pInputStateStatic ) 
					pInputStateStatic->SetShow ( false );
			}
			break;
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
		default:
			{
				
			}break;
		}
	}
	return;
}

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편

void CX2SecurityPad::ShuffleNumber()
{
	if( m_pDLGSecurityPad == NULL)
		return;

	for(int i=0; i<_CONST_SECURITY_PAD_::MAX_PASSWORD_BUTTON_EXCEPT_RESET_BUTTON; i++)
	{
		m_iButtonSequence[i] = -1;
	}
	bool bSequenceCheck[_CONST_SECURITY_PAD_::MAX_PASSWORD_BUTTON_EXCEPT_RESET_BUTTON] = {false,};
	srand( (unsigned) time( NULL ) );

	for(int i=0; i < _CONST_SECURITY_PAD_::MAX_PASSWORD_BUTTON_EXCEPT_RESET_BUTTON; i++)
	{
		int iRandOffset = rand() % (_CONST_SECURITY_PAD_::MAX_PASSWORD_BUTTON_EXCEPT_RESET_BUTTON - i);

		for(int j=0; j< _CONST_SECURITY_PAD_::MAX_PASSWORD_BUTTON_EXCEPT_RESET_BUTTON; j++)
		{
			if(bSequenceCheck[j] == false)
			{
				if(iRandOffset > 0)
				{
					iRandOffset--;
				}
				else
				{
					bSequenceCheck[j] = true;
					m_iButtonSequence[i] = j;
					break;
				}
			}
		}
	}

	for(int i=0; i<_CONST_SECURITY_PAD_::MAX_PASSWORD_BUTTON_EXCEPT_RESET_BUTTON; i++)
	{
		CKTDGUIButton* pButtonDialNumber = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl(GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"ButtonDialNumber" ), i ) ) )  );
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		WCHAR wNormalTextrueKeyName[256] = {0};
		StringCchPrintf( wNormalTextrueKeyName, 256, L"bt_security_%d_N", m_iButtonSequence[i] );


		WCHAR wOverTextrueKeyName[256] = {0};
		StringCchPrintf( wOverTextrueKeyName, 256, L"bt_security_%d_OVER", m_iButtonSequence[i] );

		pButtonDialNumber->SetNormalTex(L"DLG_UI_Button_NEW_GUI_security_01.tga", wNormalTextrueKeyName );
		pButtonDialNumber->SetOverTex(L"DLG_UI_Button_NEW_GUI_security_01.tga", wOverTextrueKeyName );
		pButtonDialNumber->SetDownTex(L"DLG_UI_Button_NEW_GUI_security_01.tga", wOverTextrueKeyName );
#else  // REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		wstring wstrNormalTextrueKeyName = GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"bt_normal_" ), m_iButtonSequence[i] ) );
		wstring wstrOverTextrueKeyName = GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"bt_over_" ), m_iButtonSequence[i] ) ); 

		pButtonDialNumber->SetNormalTex(L"DLG_ui_button18.tga", wstrNormalTextrueKeyName.c_str() );
		pButtonDialNumber->SetOverTex(L"DLG_ui_button18.tga", wstrOverTextrueKeyName.c_str() );
		pButtonDialNumber->SetDownTex(L"DLG_ui_button18.tga", wstrOverTextrueKeyName.c_str() );
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh		

	}
	return;
}

#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편

void CX2SecurityPad::ShuffleNumber()
{
	if( m_pDLGSecurityPad == NULL)
		return;

	for(int i=0; i<12; i++)
	{
		m_iButtonSequence[i] = -1;
	}
	bool bSequenceCheck[12] = {false,};
	srand( (unsigned) time( NULL ) );

	for(int i=0; i < 12; i++)
	{
		int iRandOffset = rand() % (12 - i);

		for(int j=0; j< 12; j++)
		{
			if(bSequenceCheck[j] == false)
			{
				if(iRandOffset > 0)
				{
					iRandOffset--;
				}
				else
				{
					bSequenceCheck[j] = true;
					m_iButtonSequence[i] = j;
					break;
				}
			}
		}
	}

	for(int i=0; i<12; i++)
	{
		CKTDGUIButton* pButtonDialNumber = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl(GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"ButtonDialNumber" ), i ) ) )  );
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
#ifdef SECURITY_PAD_ADD_EXCEPTION_AND_BUG_FIX
		if( pButtonDialNumber == NULL )	// 임규수 일본 추가 예외 처리
			return;
#endif //SECURITY_PAD_ADD_EXCEPTION_AND_BUG_FIX			
		WCHAR wNormalTextrueKeyName[256] = {0};
		StringCchPrintf( wNormalTextrueKeyName, 256, L"bt_security_%d_N", m_iButtonSequence[i] );


		WCHAR wOverTextrueKeyName[256] = {0};
		StringCchPrintf( wOverTextrueKeyName, 256, L"bt_security_%d_OVER", m_iButtonSequence[i] );

		pButtonDialNumber->SetNormalTex(L"DLG_UI_Button_NEW_GUI_security_01.tga", wNormalTextrueKeyName );
		pButtonDialNumber->SetOverTex(L"DLG_UI_Button_NEW_GUI_security_01.tga", wOverTextrueKeyName );
		pButtonDialNumber->SetDownTex(L"DLG_UI_Button_NEW_GUI_security_01.tga", wOverTextrueKeyName );
#else  // REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
#ifdef SECURITY_PAD_ADD_EXCEPTION_AND_BUG_FIX
		if( pButtonDialNumber == NULL )	// 임규수 일본 추가 예외 처리
			return;
#endif //SECURITY_PAD_ADD_EXCEPTION_AND_BUG_FIX			
		wstring wstrNormalTextrueKeyName = GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"bt_normal_" ), m_iButtonSequence[i] ) );
		wstring wstrOverTextrueKeyName = GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"bt_over_" ), m_iButtonSequence[i] ) ); 

		pButtonDialNumber->SetNormalTex(L"DLG_ui_button18.tga", wstrNormalTextrueKeyName.c_str() );
		pButtonDialNumber->SetOverTex(L"DLG_ui_button18.tga", wstrOverTextrueKeyName.c_str() );
		pButtonDialNumber->SetDownTex(L"DLG_ui_button18.tga", wstrOverTextrueKeyName.c_str() );
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh		

	}
	return;
}

#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편

HRESULT CX2SecurityPad::OnFrameMove(double fTime, float fElapsedTime)
{


	return S_OK;
}
bool CX2SecurityPad::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{   
	if(m_bShow == false)
		return false;

	return false;
}
void CX2SecurityPad::ClearInputPassword()
{
	m_iPasswordIndex = 0;
	for(int i=0; i<7; i++)
	{
		m_cPassword[i] = 0;
	}
	UpdateUIMarble();
	return;
}

void CX2SecurityPad::ProcessPassword()
{
	switch(m_eState)
	{
	case SPS_GAME_START: // Phase Count 1
		{
			m_wstrPassword_Temp1 = m_cPassword;

			// 입력이 없을 경우 무반응
			if(m_wstrPassword_Temp1 == L"")
				return;
#ifdef FIX_REFORM_ENTRY_POINT_7TH		// 김종훈, 진입 구조 개편 7차, kimjh
			Handler_EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_REQ ();
#else // FIX_REFORM_ENTRY_POINT_7TH		// 김종훈, 진입 구조 개편 7차, kimjh
			Handler_EGS_AUTH_SECOND_SECURITY_REQ();
#endif // FIX_REFORM_ENTRY_POINT_7TH	// 김종훈, 진입 구조 개편 7차, kimjh
			
		} break;
	case SPS_CREATE_PASSWORD: // Phase Count 2
		{
			if(m_iPhase == 0)
			{
				m_wstrPassword_Temp1 = m_cPassword;

				// 입력이 없을 경우 무반응
				if(m_wstrPassword_Temp1 == L"")
					return;

				m_iPhase = 1;
				ClearInputPassword();
				UpdateUIMarble();
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
				CKTDGUIStatic* pInputstateStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"InputStateStatic" ) );
				if ( NULL != pInputstateStatic )
				{					
					for ( int idx = 0; idx < pInputstateStatic->GetPictureNum(); ++idx )
					{
						if ( NULL != pInputstateStatic->GetPicture(idx) )
						{
							pInputstateStatic->GetPicture(idx)->SetShow( false );
						}
					}
					if ( NULL != pInputstateStatic->GetPicture(1) )
						pInputstateStatic->GetPicture(1)->SetShow ( true );
				}
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
				m_pStaticMemo->SetString(0, GET_STRING( STR_ID_12538 ));
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
				ShuffleNumber();
			}
			else if(m_iPhase == 1)
			{
				wstring wstrVerifyPassword = m_cPassword;
				
				// 입력이 없을 경우 무반응
				if(wstrVerifyPassword == L"")
					return;

				if(m_wstrPassword_Temp1 == wstrVerifyPassword)
				{
					Handler_EGS_CREATE_SECOND_SECURITY_REQ();
				}
				else
				{
					// 일치하지 않습니다.
					m_iPhase = 0;
					m_wstrPassword_Temp1.clear();
					m_wstrPassword_Temp2.clear();
					ClearInputPassword();
					UpdateUIMarble();

#ifdef REFORM_ENTRY_POINT
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_12539 ), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else //REFORM_ENTRY_POINT
					m_pStaticMemo->SetString(0, L"");
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12539 ), g_pMain->GetNowState() );
#endif // REFORM_ENTRY_POINT
					SetShow(false);
				}
			}
		} break;
	case SPS_RELEASE_PASSWORD:  // Phase Count 1
		{
			m_wstrPassword_Temp1 = m_cPassword;

			// 입력이 없을 경우 무반응
			if(m_wstrPassword_Temp1 == L"")
				return;

			Handler_EGS_DELETE_SECOND_SECURITY_REQ();
		} break;
	case SPS_CHANGE_PASSWORD:  // Phase Count 3
		{
			if(m_iPhase == 0)
			{
				m_wstrPassword_Temp1 = m_cPassword;

				// 입력이 없을 경우 무반응
				if(m_wstrPassword_Temp1 == L"")
					return;

				Handler_EGS_COMPARE_SECOND_SECURITY_PW_REQ();
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
				CKTDGUIStatic* pInputstateStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"InputStateStatic" ) );
				if ( NULL != pInputstateStatic )
				{					
					for ( int idx = 0; idx < pInputstateStatic->GetPictureNum(); ++idx )
					{
						if ( NULL != pInputstateStatic->GetPicture(idx) )
						{
							pInputstateStatic->GetPicture(idx)->SetShow( false );
						}
					}
					if ( NULL != pInputstateStatic->GetPicture(0) )
						pInputstateStatic->GetPicture(0)->SetShow ( true );
				}
#endif // REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
			}
			else if(m_iPhase == 1)
			{
				m_wstrPassword_Temp2 = m_cPassword;

				// 입력이 없을 경우 무반응
				if(m_wstrPassword_Temp2 == L"")
					return;

				m_iPhase = 2;
				ClearInputPassword();
				UpdateUIMarble();
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
				CKTDGUIStatic* pInputstateStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"InputStateStatic" ) );
				if ( NULL != pInputstateStatic )
				{					
					for ( int idx = 0; idx < pInputstateStatic->GetPictureNum(); ++idx )
					{
						if ( NULL != pInputstateStatic->GetPicture(idx) )
						{
							pInputstateStatic->GetPicture(idx)->SetShow( false );
						}
					}
					if ( NULL != pInputstateStatic->GetPicture(1) )
						pInputstateStatic->GetPicture(1)->SetShow ( true );
				}
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
				m_pStaticMemo->SetString(0, GET_STRING( STR_ID_12538 ));
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
				ShuffleNumber();
			}
			else if(m_iPhase == 2)
			{
				wstring wstrVerifyPassword = m_cPassword;
				
				// 입력이 없을 경우 무반응
				if(wstrVerifyPassword == L"")
					return;

				if(m_wstrPassword_Temp2 == wstrVerifyPassword)
				{
					Handler_EGS_CHANGE_SECOND_SECURITY_PW_REQ();
				}
				else
				{
					// 일치하지 않습니다.
					m_iPhase = 0;
					m_wstrPassword_Temp1.clear();
					m_wstrPassword_Temp2.clear();
					ClearInputPassword();
					UpdateUIMarble();

#ifdef REFORM_ENTRY_POINT
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_12539 ), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else //REFORM_ENTRY_POINT
					m_pStaticMemo->SetString(0, L"");
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12539 ), g_pMain->GetNowState() );
#endif // REFORM_ENTRY_POINT

					SetShow(false);
				}
			}
		} break;

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	case SPC_SELECT_STATE :
		{
			CKTDGUIStatic* pInputstateStatic = static_cast< CKTDGUIStatic* >(m_pDLGSecurityPad->GetControl( L"InputStateStatic" ) );
			if ( NULL != pInputstateStatic )
			{					
				for ( int idx = 0; idx < pInputstateStatic->GetPictureNum(); ++idx )
				{
					if ( NULL != pInputstateStatic->GetPicture(idx) )
					{
						pInputstateStatic->GetPicture(idx)->SetShow( false );
					}
				}
				if ( NULL != pInputstateStatic->GetPicture(0) )
					pInputstateStatic->GetPicture(0)->SetShow ( true );
			}
		}
		break;
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
	}

	return;
}

bool CX2SecurityPad::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{    

	switch ( wParam )
	{
	case SPUM_EXIT:
		{
			if(m_eState == SPS_GAME_START)
			{
				//CX2State::SUCM_EXIT
#ifdef REFORM_ENTRY_POINT
				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ), GET_STRING( STR_ID_12541 ), CX2State::SUCM_EXIT, g_pMain->GetNowState(), -1, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
#else //REFORM_ENTRY_POINT
				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12541 ), CX2State::SUCM_EXIT, g_pMain->GetNowState());
#endif //REFORM_ENTRY_POINT
			}
			else
			{
				SetShow( false );
			}
			return true;
		} break;
	case SPUM_OK:
		{
			ProcessPassword();
			UpdateUIMarble();
			return true;
		} break;
	case SPUM_CANCEL:
		{
#ifdef SECURITY_PAD_ADD_EXCEPTION_AND_BUG_FIX
			if( m_iPhase == 0 )		// 임규수 일본 추가
			m_wstrPassword_Temp1.clear();
			if( m_iPhase != 2 )		// 임규수 일본 추가
			m_wstrPassword_Temp2.clear();
#else //SECURITY_PAD_ADD_EXCEPTION_AND_BUG_FIX
			m_wstrPassword_Temp1.clear();
			m_wstrPassword_Temp2.clear();
#endif //SECURITY_PAD_ADD_EXCEPTION_AND_BUG_FIX			
			ClearInputPassword();
			UpdateUIMarble();
			return true;
		} break;
	case SPUM_DIAL_NUMBER_0:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[0] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[0] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DIAL_NUMBER_1:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[1] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[1] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DIAL_NUMBER_2:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[2] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[2] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DIAL_NUMBER_3:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[3] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[3] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DIAL_NUMBER_4:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[4] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[4] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DIAL_NUMBER_5:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[5] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[5] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DIAL_NUMBER_6:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[6] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[6] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DIAL_NUMBER_7:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[7] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[7] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DIAL_NUMBER_8:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[8] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[8] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DIAL_NUMBER_9:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[9] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[9] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DIAL_NUMBER_10:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[10] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[10] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DIAL_NUMBER_11:
		{
			if(m_iPasswordIndex < 6 && m_iButtonSequence[11] < 10)
			{
				m_cPassword[m_iPasswordIndex] = m_iButtonSequence[11] + 48;
				m_iPasswordIndex++;
				UpdateUIMarble();
			}
			return true;
		} break;
	case SPUM_DELETE_PASSWORD_VERIFY_OK:
		{
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			Handler_EGS_DELETE_SECOND_SECURITY_VERIFY_REQ();
		} break;
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	case SPUM_CHANGE_STATE_CHANGE_PASSWORD :
		g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPS_CHANGE_PASSWORD, false);
		break;
	case SPUM_CHANGE_STATE_RELEASE_PASSWORD :
		g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPS_RELEASE_PASSWORD, false);
		break;
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

	}
	return false;
}
void CX2SecurityPad::UpdateUIMarble()
{
	CKTDGUIStatic* pStaticSignal = static_cast< CKTDGUIStatic* >( m_pDLGSecurityPad->GetControl( L"StaticSignal" )  );

	for(int i=0; i<6; i++)
	{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
		if(i< m_iPasswordIndex)
		{
			if ( NULL != pStaticSignal->GetPicture(i) )
				pStaticSignal->GetPicture(i)->SetTex( L"DLG_UI_Common_Texture_GUI_FLOW_security01.tga", L"PW_DOT_YELLOW" );
		}
		else
		{
			if ( NULL != pStaticSignal->GetPicture(i) )
				pStaticSignal->GetPicture(i)->SetTex( L"DLG_UI_Common_Texture_GUI_FLOW_security01.tga", L"PW_DOT_GRAY" );

		}
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
		if(i< m_iPasswordIndex)
			pStaticSignal->GetPicture(i)->SetTex( L"DLG_ui_button08.tga", L"radio_over" );
		else
			pStaticSignal->GetPicture(i)->SetTex( L"DLG_ui_button08.tga", L"radio_normal" );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
	}

	CKTDGUIButton* pButtonOk = static_cast< CKTDGUIButton* >( m_pDLGSecurityPad->GetControl( L"ButtonOK" )  );
	if(m_iPasswordIndex < 4)
	{
		pButtonOk->SetEnable( false );
	}
	else
	{
		pButtonOk->SetEnable( true );
	}
	return;
}

bool CX2SecurityPad::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case EGS_AUTH_SECOND_SECURITY_ACK:
		{
			return Handler_EGS_AUTH_SECOND_SECURITY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_CREATE_SECOND_SECURITY_ACK:
		{
			return Handler_EGS_CREATE_SECOND_SECURITY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_DELETE_SECOND_SECURITY_ACK:
		{
			return Handler_EGS_DELETE_SECOND_SECURITY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_DELETE_SECOND_SECURITY_VERIFY_ACK:
		{
			return Handler_EGS_DELETE_SECOND_SECURITY_VERIFY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_CHANGE_SECOND_SECURITY_PW_ACK:
		{
			return Handler_EGS_CHANGE_SECOND_SECURITY_PW_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_COMPARE_SECOND_SECURITY_PW_ACK:
		{
			return Handler_EGS_COMPARE_SECOND_SECURITY_PW_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#ifdef FIX_REFORM_ENTRY_POINT_7TH		// 김종훈, 진입 구조 개편 7차, kimjh
	case EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK:
		{
			return Handler_EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_GET_SECOND_SECURITY_INFO_ACK :
	{
		return Handler_EGS_GET_SECOND_SECURITY_INFO_ACK ( hWnd, uMsg, wParam, lParam );
	} break;
#endif // FIX_REFORM_ENTRY_POINT_7TH	// 김종훈, 진입 구조 개편 7차, kimjh
#ifdef SERV_SECOND_SECURITY_K3R_AUTH
	case ECH_SECURITY_AUTH_ACK:
		{
			return Handler_ECH_SECURITY_AUTH_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_SECOND_SECURITY_K3R_AUTH
	}

	return false;
}

#ifdef SERV_SECOND_SECURITY_K3R_AUTH
bool CX2SecurityPad::Handler_ECH_SECURITY_AUTH_REQ()
{
	// 여긴 미국용 2차 보안 패드 알림 코드임
	KECH_SECURITY_AUTH_REQ kPacket;
	kPacket.m_iSecurityAuthType = KECH_SECURITY_AUTH_REQ::SAT_CHK_SECOND_SECURITY;
	kPacket.m_iUserUID = g_pData->GetMyUser()->GetUID();
	kPacket.m_wstrUserID = g_pInstanceData->GetUserID();

	KServerGroupInfo kInfo = g_pMain->GetPickedChannelServer();

	if( g_pData->GetServerProtocol()->ConnectedToChannelServer( kInfo.m_kServerIP.c_str(), kInfo.m_usMasterPort ) == true )		
	{
		g_pData->GetServerProtocol()->SendChPacket( ECH_SECURITY_AUTH_REQ, kPacket );
		g_pMain->AddServerPacket( ECH_SECURITY_AUTH_ACK );
	}

	return true;
}

bool CX2SecurityPad::Handler_ECH_SECURITY_AUTH_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KECH_SECURITY_AUTH_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( ECH_SECURITY_AUTH_ACK ) == true )
	{
		g_pData->GetServerProtocol()->DisconnectFromChannelServer();

		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	
			// 인증 성공
			g_pMain->GetSecurityPad()->SetShow( true, CX2SecurityPad::SPS_CREATE_PASSWORD );
			return true;
		}
		else // 한가지 경우 뿐이므로 일단 코드를 정하지 않습니다.
		{
			// 질문 설정용 홈페이지 띄워주기
			ShellExecuteA( NULL, "open", kEvent.m_strSecondSecurityURL.c_str(), NULL, NULL, SW_SHOW );
			return true;
		}
	}

	return false;
}
#endif //SERV_SECOND_SECURITY_K3R_AUTH

/////////////////////////////////////////////////////////////////
// EGS_AUTH_SECOND_SECURITY ///////////////////////////////////
bool CX2SecurityPad::Handler_EGS_AUTH_SECOND_SECURITY_REQ()  // Phase Count 1
{
	KEGS_AUTH_SECOND_SECURITY_REQ kPacket;

	kPacket.m_iUserUID = g_pData->GetMyUser()->GetUID();
	
#ifdef FIX_REFORM_ENTRY_POINT_7TH		// 김종훈, 진입 구조 개편 7차, kimjh
	kPacket.m_wstrSecondPW = m_wstrAutoAuthorityPassword;
#else // FIX_REFORM_ENTRY_POINT_7TH		// 김종훈, 진입 구조 개편 7차, kimjh
	kPacket.m_wstrSecondPW = m_cPassword;
#endif // FIX_REFORM_ENTRY_POINT_7TH	// 김종훈, 진입 구조 개편 7차, kimjh
	g_pData->GetServerProtocol()->SendPacket( EGS_AUTH_SECOND_SECURITY_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_AUTH_SECOND_SECURITY_ACK); 
	return true;
}
bool CX2SecurityPad::Handler_EGS_AUTH_SECOND_SECURITY_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_AUTH_SECOND_SECURITY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_AUTH_SECOND_SECURITY_ACK ) == true )
	{
		if( kEvent.m_iOK == NetError::ERR_SECOND_SECURITY_AUTH_FAILED)
		{

#ifdef FIX_REFORM_ENTRY_POINT_7TH
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_REPLACED_STRING( ( STR_ID_12351, "i", kEvent.m_iFailedCount ) ), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else // FIX_REFORM_ENTRY_POINT_7TH
	#ifdef REFORM_ENTRY_POINT
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_REPLACED_STRING( ( STR_ID_12351, "i", kEvent.m_iFailedCount ) ), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
	#else //REFORM_ENTRY_POINT
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_12351, "i", kEvent.m_iFailedCount ) ), g_pMain->GetNowState() );
	#endif //REFORM_ENTRY_POINT
	
			//창을 유지시킨채 계속 물어야한다.
			m_iPhase = 0;
			m_wstrPassword_Temp1.clear();
			m_wstrPassword_Temp2.clear();
			ClearInputPassword();
			UpdateUIMarble();
	#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
			m_pStaticMemo->SetString(0, GET_STRING( STR_ID_12537 ));
	#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
#endif // // FIX_REFORM_ENTRY_POINT_7TH
		}
		else if( kEvent.m_iOK == NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT )
		{
	#ifdef REFORM_ENTRY_POINT
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_12352 ), g_pMain->GetNowState(), CX2State::SUCM_EXIT, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
	#else //REFORM_ENTRY_POINT
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12352 ), g_pMain->GetNowState(), CX2State::SUCM_EXIT );
	#endif //REFORM_ENTRY_POINT
		}
		else if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	

#ifdef FIX_REFORM_ENTRY_POINT_7TH		// kimjh, 진입 구조 개편, 7차 수정 사항 ( 보안 큐브 및 복귀 유저 보상 받을 수 있도록 수정 )
			if ( g_pMain->GetNowStateID () == CX2Main::XS_SERVER_SELECT )
			{
				CX2StateServerSelect* pStateServerSelect = static_cast<CX2StateServerSelect*> ( g_pMain->GetNowState() );
				pStateServerSelect->Handler_EGS_SELECT_UNIT_REQ();
			}
#else // FIX_REFORM_ENTRY_POINT_7TH		// kimjh, 진입 구조 개편, 7차 수정 사항 ( 보안 큐브 및 복귀 유저 보상 받을 수 있도록 수정 )
	#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
			if ( m_wstrAutoAuthorityPassword.size() < 3 )
			{
				m_wstrAutoAuthorityPassword = m_cPassword;
		#ifdef FIX_REFORM_ENTRY_POINT_2ND // 김종훈, 진입 구조 개편 수정 2차
				m_bIsAutoAuthorityPassworBegin = true;
		#endif // FIX_REFORM_ENTRY_POINT_2TH // 김종훈, 진입 구조 개편 수정 2차
			}
	#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

			// 인증 성공
			SetShow(false);
			CX2StateServerSelect* pStateServerSelect = (CX2StateServerSelect*)g_pMain->GetNowState();
			pStateServerSelect->SetEnableUnitSelect(true);
	#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
			CX2StateServerSelect* pState = static_cast<CX2StateServerSelect*>(g_pMain->GetNowState());
			pState->Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ();
	#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		
#endif // FIX_REFORM_ENTRY_POINT_7TH	// kimjh, 진입 구조 개편, 7차 수정 사항 ( 보안 큐브 및 복귀 유저 보상 받을 수 있도록 수정 )

			
			return true;
		}
	}
	return false;
}
/////////////////////////////////////////////////////////////////
// EGS_CREATE_SECOND_SECURITY ///////////////////////////////////
bool CX2SecurityPad::Handler_EGS_CREATE_SECOND_SECURITY_REQ()  // Phase Count 2
{
	KEGS_CREATE_SECOND_SECURITY_REQ kPacket;
	kPacket.m_wstrSecondPW = m_cPassword;

	g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_SECOND_SECURITY_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_CREATE_SECOND_SECURITY_ACK); 

	return true;
}
bool CX2SecurityPad::Handler_EGS_CREATE_SECOND_SECURITY_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_SECOND_SECURITY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_CREATE_SECOND_SECURITY_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	
			SetUseSecondPW(true);

#ifdef REFORM_ENTRY_POINT
			m_wstrAutoAuthorityPassword = m_cPassword;
#ifdef FIX_REFORM_ENTRY_POINT_2ND // 김종훈, 진입 구조 개편 수정 2차
			m_bIsAutoAuthorityPassworBegin = true;
#endif // FIX_REFORM_ENTRY_POINT_2TH // 김종훈, 진입 구조 개편 수정 2차
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING(STR_ID_12542), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else //REFORM_ENTRY_POINT
			CX2StateOption* pStateOption = (CX2StateOption*)g_pMain->GetNowState();
			pStateOption->ShowSecurityOption(true);
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12542 ), g_pMain->GetNowState() );
#endif //REFORM_ENTRY_POINT
			
			

			SetShow(false);
			return true;
		}
		else
		{
			SetShow(false);
		}
	}
	return false;
}
/////////////////////////////////////////////////////////////////
// EGS_DELETE_SECOND_SECURITY ///////////////////////////////////
bool CX2SecurityPad::Handler_EGS_DELETE_SECOND_SECURITY_REQ()  // Phase Count 1
{
	KEGS_DELETE_SECOND_SECURITY_REQ kPacket;
	kPacket.m_wstrSecondPW = m_cPassword;
	m_wstrPassword_Delete = m_cPassword;

	g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_SECOND_SECURITY_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_DELETE_SECOND_SECURITY_ACK); 

	return true;
}

bool CX2SecurityPad::Handler_EGS_DELETE_SECOND_SECURITY_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DELETE_SECOND_SECURITY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_DELETE_SECOND_SECURITY_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	
#ifdef REFORM_ENTRY_POINT
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ), GET_STRING( STR_ID_12543 ), SPUM_DELETE_PASSWORD_VERIFY_OK, g_pMain->GetNowState(), -1, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
#else //REFORM_ENTRY_POINT
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12543 ), SPUM_DELETE_PASSWORD_VERIFY_OK, g_pMain->GetNowState());
#endif //REFORM_ENTRY_POINT
			return true;
		}
		else
		{
			SetShow(false);
		}
	}
	return false;
}
bool CX2SecurityPad::Handler_EGS_DELETE_SECOND_SECURITY_VERIFY_REQ()  // Phase Count 1
{
	KEGS_DELETE_SECOND_SECURITY_VERIFY_REQ kPacket;
	kPacket.m_bDelete = true;
	kPacket.m_wstrSecondPW = m_wstrPassword_Delete;
	g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_SECOND_SECURITY_VERIFY_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_DELETE_SECOND_SECURITY_VERIFY_ACK); 

	return true;
}

bool CX2SecurityPad::Handler_EGS_DELETE_SECOND_SECURITY_VERIFY_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DELETE_SECOND_SECURITY_VERIFY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_DELETE_SECOND_SECURITY_VERIFY_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	
			SetUseSecondPW(false);

#ifdef REFORM_ENTRY_POINT
			m_wstrAutoAuthorityPassword = L"";
#ifdef FIX_REFORM_ENTRY_POINT_2ND // 김종훈, 진입 구조 개편 수정 2차
			m_bIsAutoAuthorityPassworBegin = false;
#endif // FIX_REFORM_ENTRY_POINT_2TH // 김종훈, 진입 구조 개편 수정 2차
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING(STR_ID_12544), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else //REFORM_ENTRY_POINT
			CX2StateOption* pStateOption = (CX2StateOption*)g_pMain->GetNowState();
			pStateOption->ShowSecurityOption(true);
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12544 ), g_pMain->GetNowState() );
#endif //REFORM_ENTRY_POINT
			
			

			SetShow(false);
			return true;
		}
		else
		{
			SetShow(false);
		}
	}
	return false;
}
/////////////////////////////////////////////////////////////////
// EGS_CHANGE_SECOND_SECURITY_PW_VERIFY //////////////////////////////// 
bool CX2SecurityPad::Handler_EGS_COMPARE_SECOND_SECURITY_PW_REQ() // Phase Count 3
{
	KEGS_COMPARE_SECOND_SECURITY_PW_REQ kPacket;

	kPacket = m_wstrPassword_Temp1;

	g_pData->GetServerProtocol()->SendPacket( EGS_COMPARE_SECOND_SECURITY_PW_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_COMPARE_SECOND_SECURITY_PW_ACK); 

	return true;
}

bool CX2SecurityPad::Handler_EGS_COMPARE_SECOND_SECURITY_PW_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_COMPARE_SECOND_SECURITY_PW_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_COMPARE_SECOND_SECURITY_PW_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	
			m_iPhase = 1;
			ClearInputPassword();
			UpdateUIMarble();
#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
			m_pStaticMemo->SetString(0, GET_STRING( STR_ID_12540 ));
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
			
			ShuffleNumber();
		}
		else
		{
			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12545 ), g_pMain->GetNowState() );
			SetShow(false);
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////
// EGS_CHANGE_SECOND_SECURITY_PW //////////////////////////////// 
bool CX2SecurityPad::Handler_EGS_CHANGE_SECOND_SECURITY_PW_REQ() // Phase Count 3
{
	KEGS_CHANGE_SECOND_SECURITY_PW_REQ kPacket;

	kPacket.m_wstrOldSecondPW = m_wstrPassword_Temp1;
	kPacket.m_wstrNewSecondPW = m_wstrPassword_Temp2;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_SECOND_SECURITY_PW_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_CHANGE_SECOND_SECURITY_PW_ACK); 

	return true;
}

bool CX2SecurityPad::Handler_EGS_CHANGE_SECOND_SECURITY_PW_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_SECOND_SECURITY_PW_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_CHANGE_SECOND_SECURITY_PW_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	
#ifdef REFORM_ENTRY_POINT
			m_wstrAutoAuthorityPassword = m_cPassword;
#ifdef FIX_REFORM_ENTRY_POINT_2ND // 김종훈, 진입 구조 개편 수정 2차
			m_bIsAutoAuthorityPassworBegin = true;
#endif // FIX_REFORM_ENTRY_POINT_2TH // 김종훈, 진입 구조 개편 수정 2차
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING(STR_ID_12545), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else  // REFORM_ENTRY_POINT
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12545 ), g_pMain->GetNowState() );
#endif  // REFORM_ENTRY_POINT
			SetShow(false);
			return true;
		}
		else
		{
			SetShow(false);
			return true;
		}
	}
	return false;
}


#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh


void CX2SecurityPad::AutoAuthoritySecondSecurity ()
{
#ifdef FIX_REFORM_ENTRY_POINT_7TH		// 김종훈, 진입 구조 개편 7차, kimjh
	KEGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_REQ kPacket;
	kPacket = m_wstrAutoAuthorityPassword;
	g_pData->GetServerProtocol()->SendPacket( EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK ); 
#else // FIX_REFORM_ENTRY_POINT_7TH		// 김종훈, 진입 구조 개편 7차, kimjh
	KEGS_AUTH_SECOND_SECURITY_REQ kPacket;
	kPacket.m_iUserUID = g_pData->GetMyUser()->GetUID();
	kPacket.m_wstrSecondPW = m_wstrAutoAuthorityPassword;
	g_pData->GetServerProtocol()->SendPacket( EGS_AUTH_SECOND_SECURITY_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_AUTH_SECOND_SECURITY_ACK); 
#endif // FIX_REFORM_ENTRY_POINT_7TH	// 김종훈, 진입 구조 개편 7차, kimjh
	

}
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh




#endif SERV_SECOND_SECURITY

#ifdef FIX_REFORM_ENTRY_POINT_7TH		// 김종훈, 진입 구조 개편 7차, kimjh
bool CX2SecurityPad::Handler_EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_REQ()  // Phase Count 1
{
	KEGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_REQ kPacket;
	kPacket = m_cPassword;
	//kPacket.

	g_pData->GetServerProtocol()->SendPacket( EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK); 

	return true;
}
bool CX2SecurityPad::Handler_EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{


	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK ) == true )
	{
		if( kEvent.m_iOK == NetError::ERR_SECOND_SECURITY_AUTH_FAILED)
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_REPLACED_STRING( ( STR_ID_12351, "i", 1 ) ), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );

			//창을 유지시킨채 계속 물어야한다.
			m_iPhase = 0;
			m_wstrPassword_Temp1.clear();
			m_wstrPassword_Temp2.clear();
			ClearInputPassword();
			UpdateUIMarble();
#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
			m_pStaticMemo->SetString(0, GET_STRING( STR_ID_12537 ));
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

		}
		else if( kEvent.m_iOK == NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT )
		{
#ifdef REFORM_ENTRY_POINT
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_12352 ), g_pMain->GetNowState(), CX2State::SUCM_EXIT, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else //REFORM_ENTRY_POINT
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12352 ), g_pMain->GetNowState(), CX2State::SUCM_EXIT );
#endif //REFORM_ENTRY_POINT
		}
		else if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
			if ( m_wstrAutoAuthorityPassword.size() < 3 )
			{
				m_wstrAutoAuthorityPassword = m_cPassword;
#ifdef FIX_REFORM_ENTRY_POINT_2ND // 김종훈, 진입 구조 개편 수정 2차
				m_bIsAutoAuthorityPassworBegin = true;
#endif // FIX_REFORM_ENTRY_POINT_2TH // 김종훈, 진입 구조 개편 수정 2차
			}
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

			// 인증 성공
			SetShow(false);
			CX2StateServerSelect* pStateServerSelect = (CX2StateServerSelect*)g_pMain->GetNowState();
			pStateServerSelect->SetEnableUnitSelect(true);
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
			CX2StateServerSelect* pState = static_cast<CX2StateServerSelect*>(g_pMain->GetNowState());
			pState->Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ();
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
			return true;
		}
	}
	return false;
}
bool CX2SecurityPad::Handler_EGS_GET_SECOND_SECURITY_INFO_REQ()
{
	g_pData->GetServerProtocol()->SendID ( EGS_GET_SECOND_SECURITY_INFO_REQ ); 
	g_pMain->AddServerPacket( EGS_GET_SECOND_SECURITY_INFO_ACK ); 
	return true;
}
bool CX2SecurityPad::Handler_EGS_GET_SECOND_SECURITY_INFO_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_SECOND_SECURITY_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_GET_SECOND_SECURITY_INFO_ACK ) == true )
	{
		if( kEvent.m_iOK == NetError::ERR_SECOND_SECURITY_AUTH_FAILED)
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_12352 ), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
		}
		else if( kEvent.m_iOK == NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_12352 ), g_pMain->GetNowState(), CX2State::SUCM_EXIT, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
		}
		else if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	
			if ( g_pMain->GetNowStateID () == CX2Main::XS_SERVER_SELECT )
			{
				CX2StateServerSelect* pStateServerSelect = static_cast<CX2StateServerSelect*> ( g_pMain->GetNowState() );
				Handler_EGS_AUTH_SECOND_SECURITY_REQ();
			}
			return true;
		}
	}
	return false;
}
#endif // FIX_REFORM_ENTRY_POINT_7TH	// 김종훈, 진입 구조 개편 7차, kimjh