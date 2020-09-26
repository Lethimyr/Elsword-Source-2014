#include "stdafx.h"
#include "X2ChatBox.h"



#ifdef GLOBAL_CHAT_FUNCTION
#include <boost/algorithm/string.hpp>
#endif GLOBAL_CHAT_FUNCTION

CX2ChatBox::CX2ChatBox( CX2State* pNowState ) :
m_pStage( NULL ),
m_pDLGChatWindowButton( NULL ),
m_pDLGChatWindow( NULL ),
m_pDLGChatWindowEditBox( NULL ),
m_pDLGChatWindowHelper(NULL),
m_eChatWindowMode( CWM_TOTAL ),		
m_bOpenChatWindow( true ),
m_bChatWindowEnlarged( false),
m_wstrPickedUserNickName( L"" ),
m_iPickedUserUID( -1 ),
m_fRefreshTime( 0.f ),
m_bChatEnteredBefore( false ),
//m_AntiChatSpam;
m_wstrLastWhiperNickName( L"" ),
//m_vecChatHelpString;
m_bIsJustOpenChatEditBox( false ),
m_bHideChatWindowFrame( false ),
m_bOpenChatWindowButton( false ),
m_bFocusChatEditBox( false )

//{{ 허상형 : [2009/7/14] //	초기화
,
m_pDLGNasodScopeMessage( NULL ),
m_pDLGNasodScopeMessageYesNo( NULL ),
m_pDLGNasodScopeUserMenu( NULL ),
m_pSelectedItemSlot( NULL ),
m_wstrSenderName( L"" ),
m_iMegaID( 0 ),
m_iLastUsedMegaphoneTID( -1 )
//}} 허상형 : [2009/7/14] //

//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
, m_eIMEEditWindowMode( CWM_TOTAL )
, m_bPrevNativeLanguage( true )
, m_iNowHistoryIndex( 0 )
, m_bIsFirstOpenEditWindow( true )
#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.10 //	채팅창 개편
, m_pDLGTabMenu(NULL)
, m_ePopupWindowMode( CWM_TOTAL )
//m_vecQueuedChatLog;
#ifdef PLAY_EMOTION_BY_USER_SELECT
, m_fSendEmotionIdDelay(0.f)
#endif // PLAY_EMOTION_BY_USER_SELECT
{

	// 변수 초기화


	// 변수 값 할당
	m_pStage		= (CKTDXStage*) pNowState;

	// Dialogs
	m_pDLGChatWindow = new CKTDGUIDialog( m_pStage, L"DLG_ChatBox_new.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGChatWindow );
	m_pDLGChatWindow->SetMouseOverShowDlg(true);
	SetScrollBarEndPos();

	m_pDLGChatWindowEditBox = NULL;
	m_pDLGChatWindowEditBox = new CKTDGUIDialog( m_pStage, L"DLG_ChatBox_EditBox.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGChatWindowEditBox );


	m_pDLGChatWindowButton = NULL;
	m_pDLGChatWindowButton = new CKTDGUIDialog( m_pStage, L"DLG_ChatButton.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGChatWindowButton );

	m_pDLGChatWindowHelper = NULL;
	m_pDLGChatWindowHelper = new CKTDGUIDialog( m_pStage, L"DLG_UI_Chat_Short_Menu.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGChatWindowHelper );
	m_pDLGChatWindowHelper->SetShow(false);

	m_pDLGEmotionList = NULL;
	m_pDLGEmotionList = new CKTDGUIDialog( m_pStage, L"DLG_ChatBox_Emotion_List.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEmotionList );
	m_pDLGEmotionList->SetShow(false);

	//{{ 허상형 : [2009/7/20] //	나소드 메가폰 메뉴 Dialog 초기화
	m_pDLGNasodScopeUserMenu = new CKTDGUIDialog( m_pStage, L"DLG_Mega_Phone_User_Menu.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGNasodScopeUserMenu );
	m_pDLGNasodScopeUserMenu->SetShowEnable(false, false);	
	//}} 허상형 : [2009/7/20] //	나소드 메가폰 메뉴 Dialog 초기화

	//채팅탭 우클릭 메뉴
	m_pDLGTabMenu = new CKTDGUIDialog(m_pStage, L"DLG_Chatbox_Popup.lua");
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTabMenu );
	m_pDLGTabMenu->SetShowEnable(false, false);	
	// - - - - - - - - - - - - - - - - - - - - - - - - -

	// Show
	SetChatWindowEnlarged( m_bChatWindowEnlarged );
	OpenChatWindow();
	HideChatEditBox();
	//CloseChatWindow();

	m_vecChatHelpString.push_back( GET_STRING( STR_ID_44 ) );
	m_vecChatHelpString.push_back( GET_STRING( STR_ID_45 ) );
	m_vecChatHelpString.push_back( GET_STRING( STR_ID_46 ) );
	m_vecChatHelpString.push_back( GET_STRING( STR_ID_47 ) );
	m_vecChatHelpString.push_back( GET_STRING( STR_ID_48 ) );
	//m_vecChatHelpString.push_back( GET_STRING( STR_ID_49 ) );
	//{{ 허상형 : [2009/10/15] //	길드 도움말 추가
#ifdef GUILD_MANAGEMENT
	m_vecChatHelpString.push_back( GET_STRING( STR_ID_4616 ) );
#endif	//	GUILD_MANAGEMENT
	//}} 허상형 : [2009/10/15] //	길드 도움말 추가

#ifdef ADDED_RELATIONSHIP_SYSTEM
	m_vecChatHelpString.push_back( GET_STRING( STR_ID_24677 ) );
#endif // ADDED_RELATIONSHIP_SYSTEM

	m_vecChatHelpString.push_back( GET_STRING( STR_ID_50 ) );
	m_vecChatHelpString.push_back( GET_STRING( STR_ID_51 ) );
	m_vecChatHelpString.push_back( GET_STRING( STR_ID_52 ) );

	

	SetStage( m_pStage );

	//{{ 허상형 : [2009/7/25] //	초기 채팅 모드를 TOTAL로 세팅한다(텍스쳐 등을 초기 설정해놓기 위함)
	ChangeChatWindowAndIMEEditBox( GetChatWindowMode() );
	//}} 허상형 : [2009/7/25] //	초기 채팅 모드를 TOTAL로 세팅한다(텍스쳐 등을 초기 설정해놓기 위함)

//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
	SetNativeAndEngTexture( true );		// 처음 생성시에는 모국어
#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.12 //	채팅창 개편

	m_vecDeviceSound.clear();
}

CX2ChatBox::~CX2ChatBox()
{
	SAFE_DELETE_DIALOG( m_pDLGChatWindow );
	SAFE_DELETE_DIALOG( m_pDLGChatWindowButton );
	SAFE_DELETE_DIALOG( m_pDLGChatWindowEditBox );

	SAFE_DELETE_DIALOG( m_pDLGNasodScopeMessage );
	SAFE_DELETE_DIALOG( m_pDLGNasodScopeUserMenu );
	SAFE_DELETE_DIALOG( m_pDLGTabMenu );

	for(UINT i=0; i<m_vecDeviceSound.size(); ++i)
	{
		CKTDXDeviceSound *pSound = m_vecDeviceSound[i];
		if( pSound != NULL )
		{
			pSound->Stop();
			SAFE_CLOSE(pSound);
		}
	}
}

HRESULT CX2ChatBox::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	for(UINT i=0; i<m_vecDeviceSound.size(); ++i)
	{
		CKTDXDeviceSound *pSound = m_vecDeviceSound[i];
		if( pSound != NULL && pSound->IsPlaying() == false )
		{
			SAFE_CLOSE( pSound );
			m_vecDeviceSound.erase(m_vecDeviceSound.begin()+i);
			--i;
		}
	}

	m_AntiChatSpam.OnFrameMove( fTime, fElapsedTime );

	for( UINT i=0; i<m_vecQueuedChatLog.size(); i++ )
	{
		QueuedChatLog& chatLog = m_vecQueuedChatLog[i];
		chatLog.m_fRemainTime -= fElapsedTime;
		if( chatLog.m_fRemainTime <= 0.f )
		{
			AddChatLog( chatLog.m_wstrMsg.c_str(), chatLog.m_eChatPacketType, chatLog.m_coTextColor,
				chatLog.m_wstrColor, chatLog.m_bEnableColor, chatLog.m_bIsMyself, 0.f );

			m_vecQueuedChatLog.erase( m_vecQueuedChatLog.begin() + i );
			i -= 1;
		}
	}
	
	BOOL bPressReturn1 = false;
	BOOL bPressReturn2 = false;

	bPressReturn1 = g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RETURN)
#ifdef SERV_KEY_MAPPING_INT
					|| GET_KEY_STATE(GA_RETURN)
#endif // SERV_KEY_MAPPING_INT
					;

	bPressReturn2 = g_pKTDXApp->GetDIManager()->Getkeyboard()->GetExtraEnter()
#ifdef SERV_KEY_MAPPING_INT
					|| g_pKTDXApp->GetDIManager()->GetJoystic()->GetExtraEnter()
#endif // SERV_KEY_MAPPING_INT
					;

	// 일단 가열기 열었을경우 채팅창 열지 못하도록 한다.
	if(g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIInventory() != NULL)
	{
#ifdef ATTRACTION_ITEM_TEST
		if(g_pData->GetUIManager()->GetUIInventory()->GetPlayAttractionItem() == true)
		{
			bPressReturn1 = false;
			bPressReturn2 = false;			
		}
#endif
	}
	// 만일 모달 다이얼로그가 있으면 안 열리게 한다. (포커스 뺏겨서 버튼 활성화 안 되는 것 방지)
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckModalDlg() )
	{
		bPressReturn1 = false;
		bPressReturn2 = false;			
	}

	// '/r, /a, /p, /t, /h' 에 대한 처리
	if( NULL != m_pDLGChatWindow )
	{
		CKTDGUIIMEEditBox* pEdit_Chat = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );
		if( pEdit_Chat != NULL && pEdit_Chat->GetHaveFocusIn() == true )
		{
			if( pEdit_Chat->GetTextLength() == 3 )
			{
				wstring wstrText = pEdit_Chat->GetText();
#ifdef GLOBAL_CHAT_FUNCTION
				wstring wstrFunction[14]; // [0] : 도움말, [1] : 귓속말, [2] : 귓속말 응신, [3] : 전체 채팅, [4] : 파티 채팅, [5] : 길드 채팅
				
				wstrFunction[0] = GET_STRING(STR_ID_14330);
				wstrFunction[0] += L" ";
				wstrFunction[1] = GET_STRING(STR_ID_14331);
				wstrFunction[1] += L" ";
				wstrFunction[2] = GET_STRING(STR_ID_14332);
				wstrFunction[2] += L" ";
				wstrFunction[3] = GET_STRING(STR_ID_14333);
				wstrFunction[3] += L" ";
				wstrFunction[4] = GET_STRING(STR_ID_14334);
				wstrFunction[4] += L" ";
				wstrFunction[5] = GET_STRING(STR_ID_14335);
				wstrFunction[5] += L" ";
				wstrFunction[6] = GET_STRING(STR_ID_14336);
				wstrFunction[6] += L" ";
				wstrFunction[7] = GET_STRING(STR_ID_14337);
				wstrFunction[7] += L" ";
				wstrFunction[8] = GET_STRING(STR_ID_14338);
				wstrFunction[8] += L" ";
				wstrFunction[9] = GET_STRING(STR_ID_14339);
				wstrFunction[9] += L" ";
				wstrFunction[10] = GET_STRING(STR_ID_14340);
				wstrFunction[10] += L" ";
				wstrFunction[11] = GET_STRING(STR_ID_14341);
				wstrFunction[11] += L" ";
#ifdef ADDED_RELATIONSHIP_SYSTEM
				wstrFunction[12] = GET_STRING(STR_ID_30504);
				wstrFunction[12] += L" ";
				wstrFunction[13] = GET_STRING(STR_ID_30505);
				wstrFunction[13] += L" ";
#endif ADDED_RELATIONSHIP_SYSTEM
	
				if(boost::iequals(wstrText, wstrFunction[2]) || boost::iequals(wstrText, wstrFunction[8]))
				{
					wstrText = wstrFunction[1];
					wstrText += m_wstrLastWhiperNickName;
					wstrText += L" ";
					pEdit_Chat->SetText(wstrText.c_str());
				}
				else if(boost::iequals(wstrText, wstrFunction[3]) || boost::iequals(wstrText, wstrFunction[9]))
				{
					//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
					ChangeIMEEditBox( CWM_TOTAL );
					pEdit_Chat->RequestFocus();
#else	CHAT_WINDOW_IMPROV
					ChangeChatWindowAndIMEEditBox( CWM_TOTAL );
#endif	CHAT_WINDOW_IMPROV
					//}} kimhc // 2010.3.10 //	채팅창 개편

					pEdit_Chat->ClearText();
				}
				else if(boost::iequals(wstrText, wstrFunction[4]) || boost::iequals(wstrText, wstrFunction[10]))
				{
					//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
					ChangeIMEEditBox( CWM_PARTY );
					pEdit_Chat->RequestFocus();
#else	CHAT_WINDOW_IMPROV
					ChangeChatWindowAndIMEEditBox( CWM_PARTY );
#endif	CHAT_WINDOW_IMPROV
					//}} kimhc // 2010.3.10 //	채팅창 개편

					pEdit_Chat->ClearText();
				}
				else if(boost::iequals(wstrText, wstrFunction[5]) || boost::iequals(wstrText, wstrFunction[11]))
				{
					//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
					ChangeIMEEditBox( CWM_GUILD );
					pEdit_Chat->RequestFocus();
#else	CHAT_WINDOW_IMPROV
					ChangeChatWindowAndIMEEditBox( CWM_GUILD );
#endif	CHAT_WINDOW_IMPROV
					//}} kimhc // 2010.3.10 //	채팅창 개편

					pEdit_Chat->ClearText();
				}
#ifdef ADDED_RELATIONSHIP_SYSTEM
				else if(boost::iequals(wstrText, wstrFunction[12]) || boost::iequals(wstrText, wstrFunction[13]))
				{
					//{{ kimhc // 2010.3.10 //	채팅창 개편

					if ( NULL != g_pData->GetRelationshipManager() &&
						NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
					{
						switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
						{
						case SEnum::RT_MARRIED :
#ifdef	CHAT_WINDOW_IMPROV
							ChangeIMEEditBox( CWM_RELATION_MARRIED );
							pEdit_Chat->RequestFocus();
#else	CHAT_WINDOW_IMPROV
							ChangeChatWindowAndIMEEditBox( CWM_RELATION_MARRIED );
#endif	CHAT_WINDOW_IMPROV
							break;

						case SEnum::RT_COUPLE :
#ifdef	CHAT_WINDOW_IMPROV
							ChangeIMEEditBox( CWM_RELATION_COUPLE );
							pEdit_Chat->RequestFocus();
#else	CHAT_WINDOW_IMPROV
							ChangeChatWindowAndIMEEditBox( CWM_RELATION_COUPLE );
#endif	CHAT_WINDOW_IMPROV
							break;

						default :
							break;
						}
					}
					pEdit_Chat->ClearText();
				}
#endif ADDED_RELATIONSHIP_SYSTEM
				else if(boost::iequals(wstrText, wstrFunction[0]) || boost::iequals(wstrText, wstrFunction[6]))
				{
  					switch( GetChatWindowMode() )
					{
					case CWM_TOTAL:
						{
							for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
							{
								AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
							}
						} break;
					case CWM_PARTY:
						{
							for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
							{
								AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_PARTY, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
							}
						} break;

					case CWM_GUILD:
						{
							for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
							{

								AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_GUILD, D3DXCOLOR(0.5882f, 1, 0, 1), L"#C96ff00", false );
							}
						} break;
					case CWM_PERSONAL:
						{
							for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
							{
								AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_WHISPER, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
							}
						} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
					case CWM_RELATION_COUPLE :
						{	
							for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
							{
								AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
							}
						} break;		

					case CWM_RELATION_MARRIED :
						{	
							for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
							{
								AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
							}
						} break;		
#endif // ADDED_RELATIONSHIP_SYSTEM
					}

					pEdit_Chat->ClearText();
				}
#else GLOBAL_CHAT_FUNCTION
				if( wstrText[0] == L'/' && wstrText[2] == L' ' )
				{
					if( wstrText[1] == L'r' || wstrText[1] == L'R' || wstrText[1] == L'ㄱ' )
					{
						wstrText = L"/w ";
						wstrText += m_wstrLastWhiperNickName;
						wstrText += L" ";
						pEdit_Chat->SetText( wstrText.c_str() );
					}


					else if( wstrText[1] == L'a' || wstrText[1] == L'A' || wstrText[1] == L'ㅁ' )
					{

						//{{ kimhc // 2010.3.10 //	채팅창 개편
				#ifdef	CHAT_WINDOW_IMPROV
						ChangeIMEEditBox( CWM_TOTAL );
						pEdit_Chat->RequestFocus();
				#else	CHAT_WINDOW_IMPROV
						ChangeChatWindowAndIMEEditBox( CWM_TOTAL );
				#endif	CHAT_WINDOW_IMPROV
						//}} kimhc // 2010.3.10 //	채팅창 개편

						
						pEdit_Chat->ClearText();
					}
					else if( wstrText[1] == L'p' || wstrText[1] == L'P' || wstrText[1] == L'ㅔ' )
					{
						//{{ kimhc // 2010.3.10 //	채팅창 개편
				#ifdef	CHAT_WINDOW_IMPROV
						ChangeIMEEditBox( CWM_PARTY );
						pEdit_Chat->RequestFocus();
				#else	CHAT_WINDOW_IMPROV
						ChangeChatWindowAndIMEEditBox( CWM_PARTY );
				#endif	CHAT_WINDOW_IMPROV
						//}} kimhc // 2010.3.10 //	채팅창 개편

						pEdit_Chat->ClearText();
					}
					//{{ 허상형 : [2009/9/16] //	길드 단축키
#ifdef GUILD_MANAGEMENT

					else if( wstrText[1] == L'g' || wstrText[1] == L'G' || wstrText[1] == L'ㅎ' )
					{
						//{{ kimhc // 2010.3.10 //	채팅창 개편
				#ifdef	CHAT_WINDOW_IMPROV
						ChangeIMEEditBox( CWM_GUILD );
						pEdit_Chat->RequestFocus();
				#else	CHAT_WINDOW_IMPROV
						ChangeChatWindowAndIMEEditBox( CWM_GUILD );
				#endif	CHAT_WINDOW_IMPROV
						//}} kimhc // 2010.3.10 //	채팅창 개편

						pEdit_Chat->ClearText();
					}
#endif	//	GUILD_MANAGEMENT


					//}} 허상형 : [2009/9/16] //	길드 단축키

					else if( wstrText[1] == L'h' || wstrText[1] == L'H' || wstrText[1] == L'ㅗ' )
					{
						switch( GetChatWindowMode() )
						{
						case CWM_TOTAL:
							{
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
								}
							} break;
						case CWM_PARTY:
							{
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_PARTY, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
								}
							} break;

						case CWM_GUILD:
							{
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{

									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_GUILD, D3DXCOLOR(0.5882f, 1, 0, 1), L"#C96ff00", false );
								}
							} break;
						case CWM_PERSONAL:
							{
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_WHISPER, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
								}
							} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
						case CWM_RELATION_COUPLE :
							{	
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
								}
							} break;		
						
						case CWM_RELATION_MARRIED :
							{	
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
								}
							} break;		
#endif // ADDED_RELATIONSHIP_SYSTEM


						}
						pEdit_Chat->ClearText();
					}

#ifdef ADDED_RELATIONSHIP_SYSTEM

					else if( wstrText[1] == L'c' || wstrText[1] == L'C' || wstrText[1] == L'ㅊ' )
					{
						if ( NULL != g_pData->GetRelationshipManager() &&
							 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
						{
							switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
							{
							case SEnum::RT_MARRIED :
								ChangeIMEEditBox( CWM_RELATION_MARRIED );
								break;

							case SEnum::RT_COUPLE :
								ChangeIMEEditBox( CWM_RELATION_COUPLE );
								break;

							default :
								break;
							}
						}
						
						return true;
					}


#endif // ADDED_RELATIONSHIP_SYSTEM
				}
#endif GLOBAL_CHAT_FUNCTION
			}
		}
	}
		
	// 채팅입력중 마우스 클릭에 의해 focus를 잃은 경우에 대한 처리
	if( NULL != m_pDLGChatWindow )
	{
		CKTDGUIIMEEditBox* pEdit_Chat = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );

#ifdef ADDED_CHATBOX_DESTROY_WHEN_STRING_DATA_NOTHING

		if( NULL != pEdit_Chat )
		{
			if ( true == pEdit_Chat->GetHaveFocusIn() )		// 채팅창 클릭 처리
			{
				if ( NULL != g_pX2Game )						
				{
					g_pX2Game->SetOpenChatBox( true );		
				}
				SetFocusChatEditBox(true);					// 포커스를 활성화 한다.
			}

			else											// 채팅창이 클릭 되지 않았다.
			{
				if ( true == GetFocusChatEditBox())			// 하지만 채팅창은 열려 있다.
				{
					if ( 1 > pEdit_Chat->GetTextLength () )	// 문자열이 null 이면
					{
						HideChatEditBox();				// ChatEditBox Hide
					}
				}
				
				else									// 채팅창이 클릭되지도 않고 포커스가 활성화 되지도 않았다.
				{										// 즉, 존재할 필요가 없다. 
					if ( NULL != g_pX2Game )						
					{
						g_pX2Game->SetOpenChatBox( false ); // 채팅창도 없앤다.
					}
				}
				SetFocusChatEditBox(false);		
			}
		}
		else
		{
			SetFocusChatEditBox(false);			// 존재하지 않으면 무조건 포커스를 제거한다. 
		}


#else // ADDED_CHATBOX_DESTROY_WHEN_STRING_DATA_NOTHING
		if ( pEdit_Chat != NULL && pEdit_Chat->GetHaveFocusIn() == true )
		{
			if(g_pX2Game != NULL)
				g_pX2Game->SetOpenChatBox( true );
			SetFocusChatEditBox(true);
		}
		else
		{
			if(g_pX2Game != NULL )
				g_pX2Game->SetOpenChatBox( false );
			SetFocusChatEditBox(false);
		}

#endif // ADDED_CHATBOX_DESTROY_WHEN_STRING_DATA_NOTHING


	}


	//////////////////////////////////////////////////////////////////////////
	// 채팅 입력창 열고 닫기
	if( true == m_bIsJustOpenChatEditBox )
	{
		m_bIsJustOpenChatEditBox = false;
	}

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_PVP_GAME:
		{
			//if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RETURN) == TRUE )			
			if( (bPressReturn1 == TRUE && bPressReturn2 == FALSE) || (bPressReturn1 == FALSE && bPressReturn2 == TRUE))
			{
				if( NULL != g_pMain->GetGameEdit() && false == g_pMain->GetGameEdit()->GetShow() )
				{
					bool bCheck = true;

					CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
					if ( pControl != NULL )
					{
						if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
							pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
						{
							//if ( wcscmp( pControl->GetName(), L"IMEEditBoxChatBox" ) != 0 || 
							//	pControl->GetDialog() != m_pDLGChatWindowEditBox ||  
							//	(pControl->GetShow() == true && GetFocusChatEditBox() == true && GetChatEnteredBefore() == false) )
							{
								bCheck = false;
							}
						}
					}
					
					if ( bCheck == true )
					{
						if( GetChatEnteredBefore() == false )
						{
							m_bIsJustOpenChatEditBox = true;
							//ChangeChatWindowAndIMEEditBox( CX2ChatBox::CWM_TOTAL );
							OpenChatWindow();
							UnHideChatEditBox();
							
							//{{ kimhc // 2010.3.16 //	채팅창 개편
						#ifdef	CHAT_WINDOW_IMPROV
							// 채팅 히스토리 기능
							SetHistoryIndex( m_deqHistoryForMsg.size() );	// -1을 하지 않고 전달함 // 내부적으로 처리함
						#endif	CHAT_WINDOW_IMPROV
							//}} kimhc // 2010.3.16 //	채팅창 개편

							if( NULL != g_pX2Game )
							{
								g_pX2Game->SetOpenChatBox( true );
							}
							SetFocusChatEditBox(true);
						}
						else
						{
							HideChatEditBox();
							SetChatEnteredBefore( false );
							if( NULL != g_pX2Game )
							{
								g_pX2Game->SetOpenChatBox( false );
							}
							SetFocusChatEditBox(false);
						}
					}
				}
			}
		} break;

	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_TRAINING_GAME:
		{
			//if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RETURN) == TRUE )			
			if( (bPressReturn1 == TRUE && bPressReturn2 == FALSE) || (bPressReturn1 == FALSE && bPressReturn2 == TRUE))
			{
				if( NULL != g_pMain->GetGameEdit() &&
					false == g_pMain->GetGameEdit()->GetShow() &&
					false == g_pMain->GetIsPlayingTutorial() ) 
				{
					CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
					if( CX2Main::XS_TRAINING_GAME != g_pMain->GetNowStateID() || 
						( NULL != pDungeonGame && pDungeonGame->GetStartRealTraining() == true ) )
					{
						bool bCheck = true;

						CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
						if ( pControl != NULL )
						{
							if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
								pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
							{
								//if ( wcscmp( pControl->GetName(), L"IMEEditBoxChatBox" ) != 0 || 
								//	pControl->GetDialog() != m_pDLGChatWindowEditBox  )
								{
									bCheck = false;	
								}

							}
						}

						if ( bCheck == true )
						{
							if( false == GetChatEnteredBefore() )
							{
								m_bIsJustOpenChatEditBox = true;
								//ChangeChatWindowAndIMEEditBox( CX2ChatBox::CWM_TOTAL );
								OpenChatWindow();
								UnHideChatEditBox();
								
								//{{ kimhc // 2010.3.16 //	채팅창 개편
							#ifdef	CHAT_WINDOW_IMPROV
								// 채팅 히스토리 기능
								SetHistoryIndex( m_deqHistoryForMsg.size() );	// -1을 하지 않고 전달함 // 내부적으로 처리함
							#endif	CHAT_WINDOW_IMPROV
								//}} kimhc // 2010.3.16 //	채팅창 개편

								if( NULL != g_pX2Game )
								{
									g_pX2Game->SetOpenChatBox( true );
								}
								SetFocusChatEditBox(true);
							}
							else
							{
								HideChatEditBox();
								SetChatEnteredBefore( false );
								if( NULL != g_pX2Game )
								{
									g_pX2Game->SetOpenChatBox( false );
								}
								SetFocusChatEditBox(false);
							}
						}
					}
				}
			}
		} break;
	case CX2Main::XS_BATTLE_FIELD:
		{
			if( (bPressReturn1 == TRUE && bPressReturn2 == FALSE) || (bPressReturn1 == FALSE && bPressReturn2 == TRUE))
			{
				if( NULL != g_pMain->GetGameEdit() &&
					false == g_pMain->GetGameEdit()->GetShow() )
				{
					bool bCheck = true;

					CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
					if ( pControl != NULL )
					{
						if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
							pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
						{
							//if ( wcscmp( pControl->GetName(), L"IMEEditBoxChatBox" ) != 0 || 
							//	pControl->GetDialog() != m_pDLGChatWindowEditBox  )
							{
								bCheck = false;	
							}

						}
					}

					if ( bCheck == true )
					{
						if( false == GetChatEnteredBefore() )
						{
							m_bIsJustOpenChatEditBox = true;
							//ChangeChatWindowAndIMEEditBox( CX2ChatBox::CWM_TOTAL );
							OpenChatWindow();
							UnHideChatEditBox();

							//{{ kimhc // 2010.3.16 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
							// 채팅 히스토리 기능
							SetHistoryIndex( m_deqHistoryForMsg.size() );	// -1을 하지 않고 전달함 // 내부적으로 처리함
#endif	CHAT_WINDOW_IMPROV
							//}} kimhc // 2010.3.16 //	채팅창 개편

							if( NULL != g_pX2Game )
							{
								g_pX2Game->SetOpenChatBox( true );
							}
							SetFocusChatEditBox(true);
						}
						else
						{
							HideChatEditBox();
							SetChatEnteredBefore( false );
							if( NULL != g_pX2Game )
							{
								g_pX2Game->SetOpenChatBox( false );
							}
							SetFocusChatEditBox(false);
						}
					}
				}
			}
		} break;

	case CX2Main::XS_INVALID:
	case CX2Main::XS_START_UP:
	case CX2Main::XS_LOGIN:
	case CX2Main::XS_JOIN:
	case CX2Main::XS_LOADING:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
	case CX2Main::XS_CREATE_UNIT:
		//	case CX2Main::XS_UNIT_SELECT:
	case CX2Main::XS_PVP_RESULT:
	case CX2Main::XS_DUNGEON_RESULT:
	case CX2Main::XS_TRAINING_RESULT:
		{
		} break;


	default: // statemenu
		{
			CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
			if ( pControl != NULL )
			{
				if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
					pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
				{
					break;
				}
			}

			//if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RETURN) == TRUE )			
			if( (bPressReturn1 == TRUE && bPressReturn2 == FALSE) || (bPressReturn1 == FALSE && bPressReturn2 == TRUE))
			{
				bool bShowMarketList = false;
				CKTDGUIDialogType pDialog = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetDialog(L"DLG_UI_Market_list");
				if(pDialog != NULL)
				{
					bShowMarketList = pDialog->GetShow();
				}

				if( (g_pTFieldGame == NULL || 
					(g_pTFieldGame != NULL && g_pTFieldGame->GetJoinNpc() == false)) &&
					(bShowMarketList == false) )
				{
					if( false == GetChatEnteredBefore() )
					{
						m_bIsJustOpenChatEditBox = true;
						OpenChatWindow();
						
						if(m_pDLGChatWindowButton != NULL && m_pDLGChatWindowButton->GetShow() == true)
						{
							UnHideChatWindowFrame();
							m_bOpenChatWindowButton = true;
							//HideChatWindowFrame();
						}
						
						UnHideChatEditBox();

						//{{ kimhc // 2010.3.16 //	채팅창 개편
					#ifdef	CHAT_WINDOW_IMPROV
						// 채팅 히스토리 기능
						SetHistoryIndex( m_deqHistoryForMsg.size() );	// -1을 하지 않고 전달함 // 내부적으로 처리함
					#endif	CHAT_WINDOW_IMPROV
						//}} kimhc // 2010.3.16 //	채팅창 개편

					}
					else
					{
						HideChatEditBox();
						//HideChatWindowFrame();
						SetChatEnteredBefore( false );
						SetFocusChatEditBox(false);
					}
				}				
			}

		} break;
	}
	
	if ( NULL != g_pKTDXApp && NULL != g_pKTDXApp->GetDIManager() && NULL != g_pKTDXApp->GetDIManager()->Getkeyboard() )
		g_pKTDXApp->GetDIManager()->Getkeyboard()->SetExtraEnter(FALSE);

#ifdef SERV_KEY_MAPPING_INT
	if ( NULL != g_pKTDXApp && NULL != g_pKTDXApp->GetDIManager() && NULL != g_pKTDXApp->GetDIManager()->GetJoystic() )
		g_pKTDXApp->GetDIManager()->GetJoystic()->SetExtraEnter(FALSE);
#endif // SERV_KEY_MAPPING_INT

	// 채팅창에 마우스 오버시 윈도우 프레임 보이도록한다.	
	if(m_bOpenChatWindowButton == false)
	{
		if( GetMouseOverChatBox() == true || 
			m_pDLGChatWindowHelper->GetIsMouseOver() == true || 
			(m_pDLGEmotionList->GetShow() == true && m_pDLGEmotionList->GetIsMouseOver() == true) )
		{
			if( m_bHideChatWindowFrame == true || 
				m_pDLGChatWindowHelper->GetIsMouseOver() == true || 
				(m_pDLGEmotionList->GetShow() == true && m_pDLGEmotionList->GetIsMouseOver() == true) )
			{
#ifdef REFORM_UI_SKILLSLOT
		//스킬슬롯과 장착스킬창이 분리되어, 스킬아이콘 드래그 중에 방해 받지 않도록 예외 추가
				if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUISkillTree() &&
					false == g_pData->GetUIManager()->GetUISkillTree()->GetShow() )
#endif //REFORM_UI_SKILLSLOT
				UnHideChatWindowFrame();
			}
		}
		else
		{
			if(m_bHideChatWindowFrame == false)
				HideChatWindowFrame();

			if(m_pDLGChatWindowHelper->GetIsMouseOver() == false)
				m_pDLGChatWindowHelper->SetShowEnable(false, false);
			if(m_pDLGEmotionList->GetIsMouseOver() == false)
				m_pDLGEmotionList->SetShowEnable(false, false);
		}
	}

//{{ kimhc // 2010.3.11 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
	if ( GetFocus() == true )
	{
		ProcessInputMsgWithUpKey();					// 화살표 Up 키
		ProcessInputMsgWithDownKey();				// 화살표 Down키
		ProcessChangeIMEEditWindowByTabKey();		// Tab 키
		ChangeNativeAndEngKeyTexture();				// 한/영 키

	}


#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.11 //	채팅창 개편









	//{{ JHKang // 강정훈 // 2010/10/06 // 채팅창이 포커스를 잃었을 경우 팝업 창 상태 변경 모달리스 -> 모달
#ifdef ON_CHAT_REMAIN_FOCUS
	CKTDGUIDialogType pDialog = NULL;
	pDialog = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetDialog(L"OkAndCancelMsgBox");

	if ( true == g_pMain->IsPopUpOnChat() && false == GetFocus() )
	{
		if ( NULL != pDialog )
		{
			pDialog->SetModal( false );

			CKTDGUIButton* pOkButton = (CKTDGUIButton*)pDialog->GetControl( L"MsgBoxOkAndCancelOkButton" );
			if ( NULL != pOkButton )
				pOkButton->RequestFocus();
		}
		CloseChatWindow();
		HideChatWindowFrame();
		HideChatEditBox();
		g_pMain->SetPopUpOnChat( false );
	}

	if ( NULL == pDialog )
	{
		g_pMain->SetPopUpOnChat( false );
	}
#endif ON_CHAT_REMAIN_FOCUS
	//}} JHKang // 강정훈 // 2010/10/06 // 채팅창이 포커스를 잃었을 경우 팝업 창 상태 변경 모달리스 -> 모달
    
#ifdef PLAY_EMOTION_BY_USER_SELECT
	if( m_fSendEmotionIdDelay > 0.f )
		m_fSendEmotionIdDelay -= fElapsedTime;
#endif // PLAY_EMOTION_BY_USER_SELECT
	return S_OK;
}


HRESULT CX2ChatBox::OnFrameRender()
{
	KTDXPROFILE();

	return S_OK;
}


bool CX2ChatBox::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();
	switch ( wParam )
	{
	case PUM_CHAT_ENTER:
		{
			DialogLog( L"ChatEnterStart" );

		//{{ kimhc // 2010.3.16 //	채팅창 개편
		#ifdef	CHAT_WINDOW_IMPROV
			bool bIsEmotion		= false;
		#endif	CHAT_WINDOW_IMPROV
		//}} kimhc // 2010.3.16 //	채팅창 개편
			
			bool bIsSystemCommand = false; // 일반적인 채팅이 아니라 "/차단" 과 같은 명령어 인지			


			// 이번 프레임에 채팅입력창이 열렸으므로 enter 메세지를 무시하자.
			if( m_bIsJustOpenChatEditBox == true )			
				return true;			
						
			//{{ 허상형 : [2009/7/23] //	채팅창 닫았을때 포커스 닫히게
			CKTDGUIListBox *pListBox = GetCurrentChatbox();

			if( pListBox != NULL )
			{
				pListBox->ClearSelect();
			}

			//}} 허상형 : [2009/7/23] //	채팅창 닫았을때 포커스 닫히게
			
			m_bChatEnteredBefore = true;
			
			wstring wstrChatMsg = GetChatMessage();
#ifdef ALWAYS_IS_THIS_MESSAGESPAM_NOT
			//귓속말 도배 금지 풀기 
			if( wstrChatMsg.compare(L"/w ") == 0 )
			{
				wstrChatMsg = L"";
			}
#endif ALWAYS_IS_THIS_MESSAGESPAM_NOT
			if( wstrChatMsg.length() == 0 )
			{
				//{{ kimhc // 2010.3.18 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
				//HideChatEditBox();
#endif	CHAT_WINDOW_IMPROV
				//}} kimhc // 2010.3.18 //	채팅창 개편

				return true;
			}

			bool bEmotionCommand = IsEmotionID(wstrChatMsg);

			if(bEmotionCommand == false)
			{
#ifdef _SERVICE_
				if( m_AntiChatSpam.GetSpamBlockingTimeLeft() > 0.f )
				{
					switch( GetChatWindowMode() )
					{
					//	도배 방지 시스템 남은 시간 알림
					case CWM_TOTAL:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_53, "i", ( int )m_AntiChatSpam.GetSpamBlockingTimeLeft() ) ), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;

					case CWM_PARTY:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_53, "i", ( int )m_AntiChatSpam.GetSpamBlockingTimeLeft() ) ), KEGS_CHAT_REQ::CPT_PARTY, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;

					case CWM_PERSONAL:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_53, "i", ( int )m_AntiChatSpam.GetSpamBlockingTimeLeft() ) ), KEGS_CHAT_REQ::CPT_WHISPER, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;

					case CWM_GUILD:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_53, "i", ( int )m_AntiChatSpam.GetSpamBlockingTimeLeft() ) ), KEGS_CHAT_REQ::CPT_GUILD, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;
				
#ifdef ADDED_RELATIONSHIP_SYSTEM
					case CWM_RELATION_MARRIED:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_53, "i", ( int )m_AntiChatSpam.GetSpamBlockingTimeLeft() ) ), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;

					case CWM_RELATION_COUPLE:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_53, "i", ( int )m_AntiChatSpam.GetSpamBlockingTimeLeft() ) ), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
					}
					ClearShortCutKeyProcess();
					return true;
				}

				if( true == m_AntiChatSpam.IsThisMessageSpam( wstrChatMsg ) )
				{
					//	도배 방지 시스템 시작 알림
					switch( GetChatWindowMode() )
					{
					case CWM_TOTAL:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_55, "i", ( int )m_AntiChatSpam.GetNowSpamWarningCount() ) ), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;

					case CWM_PARTY:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_55, "i", ( int )m_AntiChatSpam.GetNowSpamWarningCount() ) ), KEGS_CHAT_REQ::CPT_PARTY, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;

					case CWM_PERSONAL:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_55, "i", ( int )m_AntiChatSpam.GetNowSpamWarningCount() ) ), KEGS_CHAT_REQ::CPT_WHISPER, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;

					case CWM_GUILD:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_55, "i", ( int )m_AntiChatSpam.GetNowSpamWarningCount() ) ), KEGS_CHAT_REQ::CPT_GUILD, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;
			#ifdef ADDED_RELATIONSHIP_SYSTEM
					case CWM_RELATION_MARRIED:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_55, "i", ( int )m_AntiChatSpam.GetNowSpamWarningCount() ) ), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;
					case CWM_RELATION_COUPLE:
						{
							AddChatLog( GET_REPLACED_STRING( ( STR_ID_55, "i", ( int )m_AntiChatSpam.GetNowSpamWarningCount() ) ), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						} break;
			#endif // ADDED_RELATIONSHIP_SYSTEM
					}
					ClearShortCutKeyProcess();
					return true;
				}
#endif
			}


		//{{ kimhc // 2010.3.10 //	채팅창 개선
		#ifdef	CHAT_WINDOW_IMPROV
			CHAT_WINDOW_MODE eChatWindowMode = GetIMEEditWindowMode();
		#else	CHAT_WINDOW_IMPROV
			CHAT_WINDOW_MODE eChatWindowMode = GetChatWindowMode();
		#endif	CHAT_WINDOW_IMPROV
		//}} kimhc // 2010.3.10 //	채팅창 개선

			
			CX2Room::ROOM_CHAT_TYPE eRoomChatType = CX2Room::RCT_ALL;
			wstring wstrWhisperTargetNickName = L"";
			UidType iWhisperTargetUID = -1;				
			bool bIsGlobalWhisper = false;
#ifdef GLOBAL_CHAT_FUNCTION
			wstring wstrCmpMsg = wstrChatMsg.substr(0,3);

			if(wstrCmpMsg.size() == 2)
			{
				wstrCmpMsg += L" ";
			}

			wstring wstrFunction[14]; // [0] : 도움말, [1] : 귓속말, [2] : 귓속말 응신, [3] : 전체 채팅, [4] : 파티 채팅, [5] : 길드 채팅

			wstrFunction[0] = GET_STRING(STR_ID_14330);
			wstrFunction[0] += L" ";
			wstrFunction[1] = GET_STRING(STR_ID_14331);
			wstrFunction[1] += L" ";
			wstrFunction[2] = GET_STRING(STR_ID_14332);
			wstrFunction[2] += L" ";
			wstrFunction[3] = GET_STRING(STR_ID_14333);
			wstrFunction[3] += L" ";
			wstrFunction[4] = GET_STRING(STR_ID_14334);
			wstrFunction[4] += L" ";
			wstrFunction[5] = GET_STRING(STR_ID_14335);
			wstrFunction[5] += L" ";
			wstrFunction[6] = GET_STRING(STR_ID_14336);
			wstrFunction[6] += L" ";
			wstrFunction[7] = GET_STRING(STR_ID_14337);
			wstrFunction[7] += L" ";
			wstrFunction[8] = GET_STRING(STR_ID_14338);
			wstrFunction[8] += L" ";
			wstrFunction[9] = GET_STRING(STR_ID_14339);
			wstrFunction[9] += L" ";
			wstrFunction[10] = GET_STRING(STR_ID_14340);
			wstrFunction[10] += L" ";
			wstrFunction[11] = GET_STRING(STR_ID_14341);
			wstrFunction[11] += L" ";
#ifdef ADDED_RELATIONSHIP_SYSTEM
			wstrFunction[12] = GET_STRING(STR_ID_30504);
			wstrFunction[12] += L" ";
			wstrFunction[13] = GET_STRING(STR_ID_30505);
			wstrFunction[13] += L" ";
#endif ADDED_RELATIONSHIP_SYSTEM

			if(boost::iequals(wstrCmpMsg, wstrFunction[1]) || boost::iequals(wstrCmpMsg, wstrFunction[7]))
			{
#ifdef CHAT_WINDOW_IMPROV
				int iIndexFirstSpace = 1;
				iIndexFirstSpace = wstrChatMsg.find( L" ", iIndexFirstSpace );

				if ( 2 != iIndexFirstSpace )
				{
					return true;
				}

				int iIndexSecondSpace = 3;
				iIndexSecondSpace = wstrChatMsg.find( L" ", iIndexSecondSpace );

				if ( 3 == iIndexSecondSpace || std::wstring::npos == iIndexSecondSpace )
					return true;

				wstrWhisperTargetNickName = wstrChatMsg.substr( iIndexFirstSpace+1, iIndexSecondSpace-iIndexFirstSpace-1 );
				wstring wstrRealChatMsg = wstrChatMsg.substr( iIndexSecondSpace+1, MAX_ROOM_CHAT_STR_LEN );

				if( wstrRealChatMsg.length() > 0 )
				{
					eChatWindowMode = CWM_PERSONAL;
					eRoomChatType = CX2Room::RCT_WHISPER;
					wstrChatMsg = wstrRealChatMsg;
				}
#else CHAT_WINDOW_IMPROV
				int iIndexFirstSpace = 1;
				iIndexFirstSpace = wstrChatMsg.find( L" ", iIndexFirstSpace );

				if( 2 == iIndexFirstSpace )
				{
					int iIndexSecondSpace = 3;
					iIndexSecondSpace = wstrChatMsg.find( L" ", iIndexSecondSpace );
					if( 3 != iIndexSecondSpace && std::wstring::npos != iIndexSecondSpace )
					{
						wstrWhisperTargetNickName = wstrChatMsg.substr( iIndexFirstSpace+1, iIndexSecondSpace-iIndexFirstSpace-1 );

						wstring wstrRealChatMsg = wstrChatMsg.substr( iIndexSecondSpace+1, MAX_ROOM_CHAT_STR_LEN );
						if( wstrRealChatMsg.length() > 0 )
						{
							eChatWindowMode = CWM_PERSONAL;
							eRoomChatType = CX2Room::RCT_WHISPER;
							wstrChatMsg = wstrRealChatMsg;
						}
					}
				}
#endif CHAT_WINDOW_IMPROV
			}
			else if(boost::iequals(wstrCmpMsg, wstrFunction[3])  || boost::iequals(wstrCmpMsg, wstrFunction[9]))
			{
#ifdef CHAT_WINDOW_IMPROV
				ChangeIMEEditBox( CWM_TOTAL );
				return true;
#else CHAT_WINDOW_IMPROV
				int iIndexFirstSpace = 1;
				iIndexFirstSpace = wstrChatMsg.find( L" ", iIndexFirstSpace );

				if( 2 == iIndexFirstSpace && std::wstring::npos != iIndexFirstSpace)
				{
					wstring wstrRealChatMsg = wstrChatMsg.substr( iIndexFirstSpace+1, MAX_ROOM_CHAT_STR_LEN );
					if( wstrRealChatMsg.length() > 0 )
					{
						eChatWindowMode = CWM_TOTAL;
						eRoomChatType = CX2Room::RCT_ALL;
						wstrChatMsg = wstrRealChatMsg;
					}
				}
#endif CHAT_WINDOW_IMPROV
			}
			else if(boost::iequals(wstrCmpMsg, wstrFunction[4]) || boost::iequals(wstrCmpMsg, wstrFunction[10]))
			{
#ifdef CHAT_WINDOW_IMPROV
				ChangeIMEEditBox( CWM_PARTY );
				return true;
#else CHAT_WINDOW_IMPROV
				int iIndexFirstSpace = 1;
				iIndexFirstSpace = wstrChatMsg.find( L" ", iIndexFirstSpace );

				if( 2 == iIndexFirstSpace && std::wstring::npos != iIndexFirstSpace)
				{
					wstring wstrRealChatMsg = wstrChatMsg.substr( iIndexFirstSpace+1, MAX_ROOM_CHAT_STR_LEN );
					if( wstrRealChatMsg.length() > 0 )
					{
						eChatWindowMode = CWM_PARTY;
						eRoomChatType = CX2Room::RCT_ALL;
						wstrChatMsg = wstrRealChatMsg;
					}
				}
#endif CHAT_WINDOW_IMPROV
			}
#ifdef GUILD_MANAGEMENT
			else if(boost::iequals(wstrCmpMsg, wstrFunction[5]) || boost::iequals(wstrCmpMsg, wstrFunction[11]))
			{
#ifdef CHAT_WINDOW_IMPROV
				ChangeIMEEditBox( CWM_GUILD );
				return true;
#else CHAT_WINDOW_IMPROV
				ChangeChatWindowAndIMEEditBox( CWM_GUILD );
				return false;
#endif CHAT_WINDOW_IMPROV
			}
#endif GUILD_MANAGEMENT
#ifdef ADDED_RELATIONSHIP_SYSTEM
			else if(boost::iequals(wstrCmpMsg, wstrFunction[12]) || boost::iequals(wstrCmpMsg, wstrFunction[13]))
			{
				//{{ kimhc // 2010.3.10 //	채팅창 개편

				if ( NULL != g_pData->GetRelationshipManager() &&
					NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
				{
					switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
					{
					case SEnum::RT_MARRIED :
#ifdef	CHAT_WINDOW_IMPROV
						ChangeIMEEditBox( CWM_RELATION_MARRIED );
						return true;
#else	CHAT_WINDOW_IMPROV
						ChangeChatWindowAndIMEEditBox( CWM_RELATION_MARRIED );
						return false;
#endif	CHAT_WINDOW_IMPROV
						break;
					case SEnum::RT_COUPLE :
#ifdef	CHAT_WINDOW_IMPROV
						ChangeIMEEditBox( CWM_RELATION_COUPLE );
						return true;
#else	CHAT_WINDOW_IMPROV
						ChangeChatWindowAndIMEEditBox( CWM_RELATION_COUPLE );
						return false;
#endif	CHAT_WINDOW_IMPROV
						break;
					default :
						break;
					}
				}
			}
#endif ADDED_RELATIONSHIP_SYSTEM
			else if(boost::iequals(wstrCmpMsg, wstrFunction[0]))
			{
				switch( GetChatWindowMode() )
				{
				case CWM_TOTAL:
					{
						for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
						{
							AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
						}
					} break;
				case CWM_PARTY:
					{
						for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
						{
							AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_PARTY, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
						}
					} break;

				case CWM_GUILD:
					{	
						for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
						{
							AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_GUILD, D3DXCOLOR(0.5882f, 1, 0, 1), L"#C96ff00", false );
						}
					} break;
				case CWM_PERSONAL:
					{
						for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
						{
							AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_WHISPER, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
						}
					} break;
				case CWM_MEGAPHONE:
					{
						for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
						{
							AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_MEGAPHONE, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
						}
					} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
				case CWM_RELATION_COUPLE:
					{
						for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
						{
							AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
						}
					} break;
				case CWM_RELATION_MARRIED:
					{
						for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
						{
							AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
						}
					} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
				}


				return false;
			}
			else
			{
				if(wstrChatMsg[0] == L'/')
				{
					wstring wstrCommand = L"";
					int iIndexFirstSpace = 1;
					iIndexFirstSpace = wstrChatMsg.find( L" ", iIndexFirstSpace );

					if( iIndexFirstSpace != -1 )
					{
						wstrCommand = wstrChatMsg.substr( 1, iIndexFirstSpace-1 );
					}
					else
					{
						wstrCommand = wstrChatMsg.substr( 1, wstrChatMsg.length() );
					}

					if( wstrCommand.length() > 0 )
					{		
						if ( wstrCommand.compare( GET_STRING( STR_ID_19199 ) ) == 0 ) //초대
						{	//wstrCommand = "닉네임 서버군"
							wstrCommand = wstrChatMsg.substr( iIndexFirstSpace+1, wstrChatMsg.length() );

							int iIndexSecondSpace = 1;//닉네임 다음 공백 인덱스 얻기
							iIndexSecondSpace = wstrCommand.find( L" ", iIndexSecondSpace );						

							wstring wstrTargetNickName = wstrCommand.substr( 0, iIndexSecondSpace );
#ifdef EXTEND_SERVER_GROUP_MASK
							int iServerGroupID = -1;
#else EXTEND_SERVER_GROUP_MASK
							//닉네임 다음 공백이 있다면 서버군 확인
							int iServerGroupID = SGI_INVALID;
							if( iIndexSecondSpace != -1 )
							{
								wstring wstrTargetServerGroup = wstrCommand.substr( iIndexSecondSpace+1, wstrCommand.length() );
								//솔레스
								if ( wstrTargetServerGroup.compare( GET_STRING( STR_ID_4836 ) ) == 0 || 
									wstrTargetServerGroup.compare( GET_STRING( STR_ID_20332 ) ) == 0 )
								{
									iServerGroupID = SGI_SOLES;
								}
								//가이아
								else if( wstrTargetServerGroup.compare( GET_STRING( STR_ID_4835 ) ) == 0 || 
									wstrTargetServerGroup.compare( GET_STRING( STR_ID_20333 ) ) == 0 )
								{
									iServerGroupID = SGI_GAIA;
								}
							}
#endif EXTEND_SERVER_GROUP_MASK

							if ( wstrTargetNickName.length() > 0 )
							{
								g_pData->GetPartyManager()->Handler_EGS_INVITE_PARTY_REQ( wstrTargetNickName, iServerGroupID);
								bIsSystemCommand = true;
							}
						}
						else if( wstrCommand.compare( GET_STRING( STR_ID_19200 ) ) == 0 ) //파티탈퇴
						{
							if( true == g_pData->GetPartyManager()->DoIHaveParty() )
							{
								g_pData->GetPartyManager()->Handler_EGS_LEAVE_PARTY_REQ( (int) NetError::NOT_LEAVE_ROOM_REASON_00 );	
								bIsSystemCommand = true;
							}	
						}
						else if(boost::iequals(wstrCommand, GET_STRING(STR_ID_4390)))				// 차단
						{								
							wstring wstrTargetNickName = wstrChatMsg.substr( iIndexFirstSpace+1, wstrChatMsg.length() );
							if( wstrTargetNickName.length() > 0 )
							{
								if( 0 != wstrTargetNickName.compare( g_pData->GetMyUser()->GetSelectUnit()->GetNickName() ) )
								{
									Handler_EGS_NEW_BLACKLIST_USER_REQ( wstrTargetNickName );
									bIsSystemCommand = true;
								}
							}

						}
						else if(boost::iequals(wstrCommand, GET_STRING(STR_ID_4391)))			// 차단해제
						{
							wstring wstrTargetNickName = wstrChatMsg.substr( iIndexFirstSpace+1, wstrChatMsg.length() );
							if( wstrTargetNickName.length() > 0 )
							{
								UidType unitUID = g_pData->GetMyUser()->GetSelectUnit()->GetBlackListUnitUID( wstrTargetNickName.c_str() );

								if( unitUID != -1 )
								{
									if( g_pData->GetMyUser()->GetSelectUnit()->GetUID() != unitUID )
									{
										Handler_EGS_DEL_BLACKLIST_USER_REQ( unitUID );
									}
									else
									{
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_56 ), m_pStage );
									}
								}
								else
								{
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_56 ), m_pStage );
								}

								bIsSystemCommand = true;
							}
						}
						else if(boost::iequals(wstrCommand, GET_STRING(STR_ID_4392)))			// 차단리스트
						{
							wstring wstrColor = L"#CFF3F4D";							// 255, 63, 77
							D3DXCOLOR coTextColor(1.f, 0.24705f, 0.30196f, 1.f);		// 255, 63, 77

							AddChatLog( GET_STRING( STR_ID_57 ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );

							const vector<KChatBlackListUnit>& vecBlackList = g_pData->GetMyUser()->GetSelectUnit()->GetBlackList();
							for( UINT i = 0 ; i < vecBlackList.size(); i++ )
							{
								AddChatLog( vecBlackList[i].m_wstrNickName.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
							}

							bIsSystemCommand = true;
						}
#ifdef SERV_COMMAND_LINE_INVITE_PVP_ROOM
						else if ( wstrCommand.compare( GET_STRING( STR_ID_17500 )) == 0 )
						{
							wstring wstrTargetNickName = wstrChatMsg.substr( iIndexFirstSpace+1, wstrChatMsg.length() );
							if ( wstrTargetNickName.length() > 0 )
							{
								switch(g_pMain->GetNowStateID())
								{
								case CX2Main::XS_PVP_ROOM:
									{
										if( NULL != g_pData && NULL != g_pData->GetMessenger() )
										{
											g_pData->GetMessenger()->Handler_EGS_INVITE_PVP_ROOM_REQ( wstrTargetNickName );
											bIsSystemCommand = true;
										}
									} break;
								default:
									break;
								}
							}
						}
#endif //SERV_COMMAND_LINE_INVITE_PVP_ROOM
					}

					//{{ kimhc // 2010.3.15 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
					bIsEmotion = IsEmotionID( wstrChatMsg );

					if ( bIsSystemCommand == false &&
						bIsEmotion == false )	// '/'로 시작되는 것중 탭변경도 아니고, 차단등의 시스템 입력도 아니면 무시
						return true;
#endif	CHAT_WINDOW_IMPROV
					//}} kimhc // 2010.3.15 //	채팅창 개편
				}
			}
#else GLOBAL_CHAT_FUNCTION
			if( wstrChatMsg[0] == L'/' )
			{
				// 채팅모드 명령어
				switch( wstrChatMsg[1] )
				{
				case L'w': 
				case L'W':
				case L'ㅈ':
					{
				//{{ kimhc // 2010.3.11 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV

						int iIndexFirstSpace = 1;
						iIndexFirstSpace = wstrChatMsg.find( L" ", iIndexFirstSpace );

						if ( 2 != iIndexFirstSpace )
						{
							return true;
						}

						int iIndexSecondSpace = 3;
						iIndexSecondSpace = wstrChatMsg.find( L" ", iIndexSecondSpace );

						if ( 3 == iIndexSecondSpace || std::wstring::npos == iIndexSecondSpace )
							return true;

						wstrWhisperTargetNickName = wstrChatMsg.substr( iIndexFirstSpace+1, iIndexSecondSpace-iIndexFirstSpace-1 );
						wstring wstrRealChatMsg = wstrChatMsg.substr( iIndexSecondSpace+1, MAX_ROOM_CHAT_STR_LEN );

						if( wstrRealChatMsg.length() > 0 )
						{
							eChatWindowMode = CWM_PERSONAL;
							eRoomChatType = CX2Room::RCT_WHISPER;
							wstrChatMsg = wstrRealChatMsg;
						}
			#else	CHAT_WINDOW_IMPROV
						int iIndexFirstSpace = 1;
						iIndexFirstSpace = wstrChatMsg.find( L" ", iIndexFirstSpace );

						if( 2 == iIndexFirstSpace )
						{
							int iIndexSecondSpace = 3;
							iIndexSecondSpace = wstrChatMsg.find( L" ", iIndexSecondSpace );
							if( 3 != iIndexSecondSpace && std::wstring::npos != iIndexSecondSpace )
							{
								wstrWhisperTargetNickName = wstrChatMsg.substr( iIndexFirstSpace+1, iIndexSecondSpace-iIndexFirstSpace-1 );

								wstring wstrRealChatMsg = wstrChatMsg.substr( iIndexSecondSpace+1, MAX_ROOM_CHAT_STR_LEN );
								if( wstrRealChatMsg.length() > 0 )
								{
									eChatWindowMode = CWM_PERSONAL;
									eRoomChatType = CX2Room::RCT_WHISPER;
									wstrChatMsg = wstrRealChatMsg;


								}
							}
						}
			#endif	CHAT_WINDOW_IMPROV
			//}} kimhc // 2010.3.11 //	채팅창 개편
						
					} 
					break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
				case L'c': 
				case L'C':
				case L'ㅊ':
					{

						int iIndexFirstSpace = 1;
						iIndexFirstSpace = wstrChatMsg.find( L" ", iIndexFirstSpace );

						if ( 2 != iIndexFirstSpace )
						{
							return true;
						}

						int iIndexSecondSpace = 3;
						iIndexSecondSpace = wstrChatMsg.find( L" ", iIndexSecondSpace );

						if ( 3 == iIndexSecondSpace || std::wstring::npos == iIndexSecondSpace )
							return true;

						wstrWhisperTargetNickName = wstrChatMsg.substr( iIndexFirstSpace+1, iIndexSecondSpace-iIndexFirstSpace-1 );
						wstring wstrRealChatMsg = wstrChatMsg.substr( iIndexSecondSpace+1, MAX_ROOM_CHAT_STR_LEN );

						if( wstrRealChatMsg.length() > 0 )
						{
							switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
							{
							case SEnum::RT_MARRIED :
								eChatWindowMode = CWM_RELATION_MARRIED;
								eRoomChatType = CX2Room::RCT_WHISPER;
								wstrChatMsg = wstrRealChatMsg;
								break;
							case SEnum::RT_COUPLE  :
								eChatWindowMode = CWM_RELATION_COUPLE;
								eRoomChatType = CX2Room::RCT_WHISPER;
								wstrChatMsg = wstrRealChatMsg;
								break;
							}
						}
					}
					break;
#endif // ADDED_RELATIONSHIP_SYSTEM
				
				case L'a':
				case L'A':
				case L'ㅁ':
					{
						//{{ kimhc // 2010.3.11 //	채팅창 개편
			#ifdef	CHAT_WINDOW_IMPROV
						ChangeIMEEditBox( CWM_TOTAL );
						return true;
			#else	CHAT_WINDOW_IMPROV
						int iIndexFirstSpace = 1;
						iIndexFirstSpace = wstrChatMsg.find( L" ", iIndexFirstSpace );

						if( 2 == iIndexFirstSpace && std::wstring::npos != iIndexFirstSpace)
						{
							wstring wstrRealChatMsg = wstrChatMsg.substr( iIndexFirstSpace+1, MAX_ROOM_CHAT_STR_LEN );
							if( wstrRealChatMsg.length() > 0 )
							{
								eChatWindowMode = CWM_TOTAL;
								eRoomChatType = CX2Room::RCT_ALL;
								wstrChatMsg = wstrRealChatMsg;
							}
						}
			#endif	CHAT_WINDOW_IMPROV
					} 
					break;

				case L'p':
				case L'P':
				case L'ㅔ':
					{
						//{{ kimhc // 2010.3.11 //	채팅창 개편
			#ifdef	CHAT_WINDOW_IMPROV
						ChangeIMEEditBox( CWM_PARTY );
						return true;
			#else	CHAT_WINDOW_IMPROV
						int iIndexFirstSpace = 1;
						iIndexFirstSpace = wstrChatMsg.find( L" ", iIndexFirstSpace );

						if( 2 == iIndexFirstSpace && std::wstring::npos != iIndexFirstSpace)
						{
							wstring wstrRealChatMsg = wstrChatMsg.substr( iIndexFirstSpace+1, MAX_ROOM_CHAT_STR_LEN );
							if( wstrRealChatMsg.length() > 0 )
							{
								eChatWindowMode = CWM_PARTY;
								eRoomChatType = CX2Room::RCT_ALL;
								wstrChatMsg = wstrRealChatMsg;
							}
						}
			#endif	CHAT_WINDOW_IMPROV
						//}} kimhc // 2010.3.11 //	채팅창 개편
					} 
					break;
					
					//{{ 허상형 : [2009/9/16] //	길드 단축키
#ifdef GUILD_MANAGEMENT
				case L'g':
				case L'G':
				case L'ㅎ':
					{
						//{{ kimhc // 2010.3.11 //	채팅창 개편
			#ifdef	CHAT_WINDOW_IMPROV
						ChangeIMEEditBox( CWM_GUILD );
						return true;
			#else	CHAT_WINDOW_IMPROV
						ChangeChatWindowAndIMEEditBox( CWM_GUILD );
						return false;
			#endif	CHAT_WINDOW_IMPROV
						//}} kimhc // 2010.3.11 //	채팅창 개편

					} break;
#endif	//	GUILD_MANAGEMENT
					//}} 허상형 : [2009/9/16] //	길드 단축키

				case L'h':
				case L'H':
				case L'ㅗ':
					{
						switch( GetChatWindowMode() )
						{
						case CWM_TOTAL:
							{
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
								}
							} break;
						case CWM_PARTY:
							{
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_PARTY, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
								}
							} break;

						case CWM_GUILD:
							{	
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_GUILD, D3DXCOLOR(0.5882f, 1, 0, 1), L"#C96ff00", false );
								}
							} break;
						case CWM_PERSONAL:
							{
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_WHISPER, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
								}
							} break;
						case CWM_MEGAPHONE:
							{
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_MEGAPHONE, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
								}
							} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
						case CWM_RELATION_COUPLE:
							{
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
								}
							} break;
						case CWM_RELATION_MARRIED:
							{
								for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
								{
									AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_RELATIONSHIP, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
								}
							} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
						}
						return false;
					} break;
					//{{ 09.08.18 태완 : 긴 명령어 받는 방식 수정
#if 0
				case L'차':
					{
						if( wstrChatMsg[2] == L'단' )
						{
							if( wstrChatMsg[3] == L'해' && wstrChatMsg[4] == L'제' && wstrChatMsg[5] == L' ' )
							{
								wstring wstrTargetNickName = wstrChatMsg.substr( 6, wstrChatMsg.length() );
								if( wstrTargetNickName.length() > 0 )
								{
									UidType unitUID = g_pData->GetMyUser()->GetSelectUnit()->GetBlackListUnitUID( wstrTargetNickName.c_str() );

									if( unitUID != -1 )
									{
										if( g_pData->GetMyUser()->GetSelectUnit()->GetUID() != unitUID )
										{
											Handler_EGS_DEL_BLACKLIST_USER_REQ( unitUID );
										}
										else
										{
											g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_56 ), m_pStage );
										}
									}
									else
									{
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_56 ), m_pStage );
									}

									bIsSystemCommand = true;
								}
							}
							else if( wstrChatMsg[3] == L' ' )
							{
								wstring wstrTargetNickName = wstrChatMsg.substr( 4, wstrChatMsg.length() );
								if( wstrTargetNickName.length() > 0 )
								{
									if( 0 != wstrTargetNickName.compare( g_pData->GetMyUser()->GetSelectUnit()->GetNickName() ) )
									{
										Handler_EGS_NEW_BLACKLIST_USER_REQ( wstrTargetNickName );
										bIsSystemCommand = true;
									}
								}
							}
							else if( wstrChatMsg[3] == L'리' && wstrChatMsg[4] == L'스' && wstrChatMsg[5] == L'트' && wstrChatMsg.length() == 6 )
							{
								wstring wstrColor = L"#CFF3F4D";							// 255, 63, 77
								D3DXCOLOR coTextColor(1.f, 0.24705f, 0.30196f, 1.f);		// 255, 63, 77

								AddChatLog( GET_STRING( STR_ID_57 ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );

								const vector<KChatBlackListUnit>& vecBlackList = g_pData->GetMyUser()->GetSelectUnit()->GetBlackList();
								for( UINT i = 0 ; i < vecBlackList.size(); i++ )
								{
									AddChatLog( vecBlackList[i].m_wstrNickName.c_str(), 
										KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
								}

								bIsSystemCommand = true;
							}
						}

					} break;
#endif
				default:
					{
						wstring wstrCommand = L"";
						int iIndexFirstSpace = 1;
						iIndexFirstSpace = wstrChatMsg.find( L" ", iIndexFirstSpace );						
						
						if( iIndexFirstSpace != -1 )
						{
							wstrCommand = wstrChatMsg.substr( 1, iIndexFirstSpace-1 );
						}
						else
						{
							wstrCommand = wstrChatMsg.substr( 1, wstrChatMsg.length() );
						}

						if( wstrCommand.length() > 0 )
						{							
							if ( wstrCommand.compare( GET_STRING( STR_ID_19199 ) ) == 0 ) //초대
							{	//wstrCommand = "닉네임 서버군"
								wstrCommand = wstrChatMsg.substr( iIndexFirstSpace+1, wstrChatMsg.length() );

								int iIndexSecondSpace = 1;//닉네임 다음 공백 인덱스 얻기
								iIndexSecondSpace = wstrCommand.find( L" ", iIndexSecondSpace );						

								wstring wstrTargetNickName = wstrCommand.substr( 0, iIndexSecondSpace );
								//닉네임 다음 공백이 있다면 서버군 확인
								int iServerGroupID = SGI_INVALID;
								if( iIndexSecondSpace != -1 )
								{
									wstring wstrTargetServerGroup = wstrCommand.substr( iIndexSecondSpace+1, wstrCommand.length() );
									//솔레스
									if ( wstrTargetServerGroup.compare( GET_STRING( STR_ID_4836 ) ) == 0 || 
										wstrTargetServerGroup.compare( GET_STRING( STR_ID_20332 ) ) == 0 )
									{
										iServerGroupID = SGI_SOLES;
									}
									//가이아
									else if( wstrTargetServerGroup.compare( GET_STRING( STR_ID_4835 ) ) == 0 || 
											wstrTargetServerGroup.compare( GET_STRING( STR_ID_20333 ) ) == 0 )
									{
										iServerGroupID = SGI_GAIA;
									}
								}

								if ( wstrTargetNickName.length() > 0 )
								{
									g_pData->GetPartyManager()->Handler_EGS_INVITE_PARTY_REQ( wstrTargetNickName, iServerGroupID);
									bIsSystemCommand = true;
								}
							}
							else if( wstrCommand.compare( GET_STRING( STR_ID_19200 ) ) == 0 ) //파티탈퇴
							{
								if( true == g_pData->GetPartyManager()->DoIHaveParty() )
								{
									g_pData->GetPartyManager()->Handler_EGS_LEAVE_PARTY_REQ( (int) NetError::NOT_LEAVE_ROOM_REASON_00 );	
									bIsSystemCommand = true;
								}	
							}
							else if( wstrCommand.compare( GET_STRING( STR_ID_4390 ) ) == 0 )		// 차단
							{								
								wstring wstrTargetNickName = wstrChatMsg.substr( iIndexFirstSpace+1, wstrChatMsg.length() );
								if( wstrTargetNickName.length() > 0 )
								{
									if( 0 != wstrTargetNickName.compare( g_pData->GetMyUser()->GetSelectUnit()->GetNickName() ) )
									{
										Handler_EGS_NEW_BLACKLIST_USER_REQ( wstrTargetNickName );
										bIsSystemCommand = true;
									}
								}

							}
							else if ( wstrCommand.compare( GET_STRING( STR_ID_4391 ) ) == 0 )		// 차단해제
							{
								wstring wstrTargetNickName = wstrChatMsg.substr( iIndexFirstSpace+1, wstrChatMsg.length() );
								if( wstrTargetNickName.length() > 0 )
								{
									UidType unitUID = g_pData->GetMyUser()->GetSelectUnit()->GetBlackListUnitUID( wstrTargetNickName.c_str() );

									if( unitUID != -1 )
									{
										if( g_pData->GetMyUser()->GetSelectUnit()->GetUID() != unitUID )
										{
											Handler_EGS_DEL_BLACKLIST_USER_REQ( unitUID );
										}
										else
										{
											g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_56 ), m_pStage );
										}
									}
									else
									{
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_56 ), m_pStage );
									}

									bIsSystemCommand = true;
								}

							}
							else if ( wstrCommand.compare( GET_STRING( STR_ID_4392 ) ) == 0 )		// 차단리스트
							{
								wstring wstrColor = L"#CFF3F4D";							// 255, 63, 77
								D3DXCOLOR coTextColor(1.f, 0.24705f, 0.30196f, 1.f);		// 255, 63, 77

								AddChatLog( GET_STRING( STR_ID_57 ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );

								const vector<KChatBlackListUnit>& vecBlackList = g_pData->GetMyUser()->GetSelectUnit()->GetBlackList();
								for( UINT i = 0 ; i < vecBlackList.size(); i++ )
								{
									AddChatLog( vecBlackList[i].m_wstrNickName.c_str(), 
										KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
								}

								bIsSystemCommand = true;

							}
#ifdef SERV_COMMAND_LINE_INVITE_PVP_ROOM
							else if ( wstrCommand.compare( GET_STRING( STR_ID_17500 )) == 0 )
							{
								wstring wstrTargetNickName = wstrChatMsg.substr( iIndexFirstSpace+1, wstrChatMsg.length() );
								if ( wstrTargetNickName.length() > 0 )
								{
									switch(g_pMain->GetNowStateID())
									{
									case CX2Main::XS_PVP_ROOM:
										{
											if( NULL != g_pData && NULL != g_pData->GetMessenger() )
											{
												g_pData->GetMessenger()->Handler_EGS_INVITE_PVP_ROOM_REQ( wstrTargetNickName );
												bIsSystemCommand = true;
											}
										} break;
									default:
										break;
									}
								}
							}
#endif //SERV_COMMAND_LINE_INVITE_PVP_ROOM
						}

				//{{ kimhc // 2010.3.15 //	채팅창 개편
				#ifdef	CHAT_WINDOW_IMPROV
						bIsEmotion = IsEmotionID( wstrChatMsg );

						if ( bIsSystemCommand == false &&
							 bIsEmotion == false )	// '/'로 시작되는 것중 탭변경도 아니고, 차단등의 시스템 입력도 아니면 무시
							return true;
				#endif	CHAT_WINDOW_IMPROV
				//}} kimhc // 2010.3.15 //	채팅창 개편

					} break;
					//}} 09.08.18 태완 : 긴 명령어 받는 방식 수정
				}
			}
#endif GLOBAL_CHAT_FUNCTION


			if( false == bIsSystemCommand ) // 일반적인 채팅인 경우
			{
	//{{ kimhc // 2010.3.10 //	채팅창 개선
#ifndef	CHAT_WINDOW_IMPROV

	//{{ 허상형 : [2009/7/18] //	메가폰 탭에서 채팅 메시지 입력시 전체 채팅 창으로 탭이동, 및 메시지 출력
				if( CWM_MEGAPHONE == GetChatWindowMode() )
				{
					if(eChatWindowMode != CWM_PERSONAL)
					{
						ChangeChatWindowAndIMEEditBox( CWM_TOTAL );
						eChatWindowMode = CWM_TOTAL;
					}					
				}
	//}} 허상형 : [2009/7/18] //	메가폰 탭에서 채팅 메시지 입력시 전체 채팅 창으로 탭이동, 및 메시지 출력

#endif	CHAT_WINDOW_IMPROV	

				if( CWM_TOTAL == eChatWindowMode ) //m_eChatWindowMode
				{
					if( CX2Room::RCT_WHISPER == eRoomChatType )
					{
						iWhisperTargetUID = GetWhisperChatTargetUID( wstrWhisperTargetNickName.c_str(), false );

						if( -1 == iWhisperTargetUID ||
							iWhisperTargetUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
						{
							bIsGlobalWhisper = true;
						}
					}

					if( false == bIsGlobalWhisper )
					{
						switch( g_pMain->GetNowStateID() )
						{
						case CX2Main::XS_PVP_ROOM:
						case CX2Main::XS_DUNGEON_ROOM:
						case CX2Main::XS_PVP_GAME:
						case CX2Main::XS_DUNGEON_GAME:
						case CX2Main::XS_SQUARE_GAME:
						case CX2Main::XS_TRAINING_GAME:
						case CX2Main::XS_VILLAGE_MAP:
						case CX2Main::XS_BATTLE_FIELD:
#ifdef ADDED_RELATIONSHIP_SYSTEM
						case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
							{
#if 0 
								if( GetKeyState( VK_SHIFT ) < 0 )
								{
									// shift + enter : 전체탭에서 파티말
									Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_PARTY, eRoomChatType, iWhisperTargetUID, wstrChatMsg );
								}
								else
								{
									Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_TOTAL, eRoomChatType, iWhisperTargetUID, wstrChatMsg );
								}
#else
								Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_TOTAL, eRoomChatType, iWhisperTargetUID, wstrChatMsg );
#endif
							} break;
						}
					}
					else
					{
						Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_WHISPER, eRoomChatType, iWhisperTargetUID, wstrChatMsg, 
							wstrWhisperTargetNickName.c_str() );
					}
				}
				else if( CWM_PARTY == eChatWindowMode )
				{
				//{{ kimhc // 2010.3.11 // 채팅창 개선
			#ifdef	CHAT_WINDOW_IMPROV
					if( CX2Room::RCT_WHISPER == eRoomChatType )
					{
						iWhisperTargetUID = GetWhisperChatTargetUID( wstrWhisperTargetNickName.c_str(), true );

						if( -1 == iWhisperTargetUID ||
							iWhisperTargetUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
						{
							bIsGlobalWhisper = true;
						}
					}

					if ( false == bIsGlobalWhisper )
					{
						switch ( g_pMain->GetNowStateID() )
						{
						case CX2Main::XS_PVP_ROOM:
						case CX2Main::XS_PVP_GAME:
							{
								Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_TOTAL, CX2Room::RCT_TEAM, iWhisperTargetUID, wstrChatMsg );
							}
							break;

						default:
							{
								if( g_pData->GetPartyManager()->GetMyPartyData()->m_iPartyUID > 0 )
								{
									Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_PARTY, eRoomChatType, iWhisperTargetUID, wstrChatMsg );
								}
								else
								{
									AddChatLog( GET_REPLACED_STRING( ( STR_ID_5117, "L", wstring( GET_STRING( STR_ID_106 ) ) ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
								}
							}
							break;
						}
					}
					else
					{
						Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_WHISPER, eRoomChatType, iWhisperTargetUID, wstrChatMsg, 
							wstrWhisperTargetNickName.c_str() );
					}
			#else	CHAT_WINDOW_IMPROV
					if( g_pData->GetPartyManager()->GetMyPartyData()->m_iPartyUID > 0 )
					{
						if( CX2Room::RCT_WHISPER == eRoomChatType )
						{
							iWhisperTargetUID = GetWhisperChatTargetUID( wstrWhisperTargetNickName.c_str(), true );

							if( -1 == iWhisperTargetUID ||
								iWhisperTargetUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
							{
								bIsGlobalWhisper = true;
							}
						}


						if( false == bIsGlobalWhisper )
						{
							Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_PARTY, eRoomChatType, iWhisperTargetUID, wstrChatMsg );
						}
						else
						{
							Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_WHISPER, eRoomChatType, iWhisperTargetUID, wstrChatMsg, 
								wstrWhisperTargetNickName.c_str() );
						}
					}
					else
					{
						if( wstrWhisperTargetNickName.length() != 0 && CX2Room::RCT_WHISPER == eRoomChatType )
						{
							Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_WHISPER, eRoomChatType, iWhisperTargetUID, wstrChatMsg, 
								wstrWhisperTargetNickName.c_str() );
						}
					}
			#endif	CHAT_WINDOW_IMPROV
				//}} kimhc // 2010.3.11 // 채팅창 개선
				}
				else if( CWM_GUILD == eChatWindowMode )
				{
					if( CX2Room::RCT_WHISPER == eRoomChatType )
					{
						iWhisperTargetUID = GetWhisperChatTargetUID( wstrWhisperTargetNickName.c_str(), false );

						if( -1 == iWhisperTargetUID ||
							iWhisperTargetUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
						{
							bIsGlobalWhisper = true;
						}
					}

					if( false == bIsGlobalWhisper )
					{
						switch( g_pMain->GetNowStateID() )
						{
						case CX2Main::XS_PVP_ROOM:
						case CX2Main::XS_DUNGEON_ROOM:
						case CX2Main::XS_PVP_GAME:
						case CX2Main::XS_DUNGEON_GAME:
						case CX2Main::XS_SQUARE_GAME:
						case CX2Main::XS_TRAINING_GAME:
						case CX2Main::XS_VILLAGE_MAP:
						case CX2Main::XS_PVP_LOBBY:
						case CX2Main::XS_BATTLE_FIELD:
#ifdef ADDED_RELATIONSHIP_SYSTEM
						case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
							{
								//{{ 허상형 : [2009/10/22] //	내가 길드 가입해있을 경우에만 패킷 전송
#ifdef GUILD_MANAGEMENT
								if( g_pData->GetGuildManager()->GetMyGuildInfo() != NULL )
#endif	//	GUILD_MANAGEMENT
								{
									Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_GUILD, eRoomChatType, iWhisperTargetUID, wstrChatMsg );
								}
								else
								{
									AddChatLog( GET_REPLACED_STRING( ( STR_ID_5117, "L", wstring( GET_STRING( STR_ID_4875 ) ) ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
								}

							} break;
						}
					}
					else
					{
						Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_WHISPER, eRoomChatType, iWhisperTargetUID, wstrChatMsg, 
							wstrWhisperTargetNickName.c_str() );
					}
				}
				else if( CWM_PERSONAL == eChatWindowMode )
				{
					if( CX2Room::RCT_WHISPER == eRoomChatType )
					{
						iWhisperTargetUID = GetWhisperChatTargetUID( wstrWhisperTargetNickName.c_str(), false );

						if( -1 == iWhisperTargetUID ||
							iWhisperTargetUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
						{
							bIsGlobalWhisper = true;
						}

						if( false == bIsGlobalWhisper )
						{
							switch( g_pMain->GetNowStateID() )
							{
							case CX2Main::XS_PVP_ROOM:
							case CX2Main::XS_DUNGEON_ROOM:
							case CX2Main::XS_PVP_GAME:
							case CX2Main::XS_DUNGEON_GAME:
							case CX2Main::XS_SQUARE_GAME:
							case CX2Main::XS_TRAINING_GAME:
							case CX2Main::XS_VILLAGE_MAP:
							case CX2Main::XS_BATTLE_FIELD:
#ifdef ADDED_RELATIONSHIP_SYSTEM
							case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
								{
									Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_TOTAL, eRoomChatType, iWhisperTargetUID, wstrChatMsg );
								} break;
							}
						}
						else
						{
							Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_WHISPER, eRoomChatType, iWhisperTargetUID, wstrChatMsg, 
								wstrWhisperTargetNickName.c_str() );
						}
					}
				}


				else if( CWM_PERSONAL == eChatWindowMode )
				{
					if( CX2Room::RCT_WHISPER == eRoomChatType )
					{
						iWhisperTargetUID = GetWhisperChatTargetUID( wstrWhisperTargetNickName.c_str(), false );

						if( -1 == iWhisperTargetUID ||
							iWhisperTargetUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
						{
							bIsGlobalWhisper = true;
						}

						if( false == bIsGlobalWhisper )
						{
							switch( g_pMain->GetNowStateID() )
							{
							case CX2Main::XS_PVP_ROOM:
							case CX2Main::XS_DUNGEON_ROOM:
							case CX2Main::XS_PVP_GAME:
							case CX2Main::XS_DUNGEON_GAME:
							case CX2Main::XS_SQUARE_GAME:
							case CX2Main::XS_TRAINING_GAME:
							case CX2Main::XS_VILLAGE_MAP:
							case CX2Main::XS_BATTLE_FIELD:
#ifdef ADDED_RELATIONSHIP_SYSTEM
							case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
								{
									Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_TOTAL, eRoomChatType, iWhisperTargetUID, wstrChatMsg );
								} break;
							}
						}
						else
						{
							Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_WHISPER, eRoomChatType, iWhisperTargetUID, wstrChatMsg, 
								wstrWhisperTargetNickName.c_str() );
						}
					}
				}


#ifdef ADDED_RELATIONSHIP_SYSTEM
				if( CWM_RELATION_COUPLE == eChatWindowMode || 
					CWM_RELATION_MARRIED == eChatWindowMode ) //m_eChatWindowMode
				{					
					if( CX2Room::RCT_WHISPER == eRoomChatType )
					{
						iWhisperTargetUID = GetWhisperChatTargetUID( wstrWhisperTargetNickName.c_str(), false );

						if( -1 == iWhisperTargetUID ||
							iWhisperTargetUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
						{
							bIsGlobalWhisper = true;
						}
					}

					if( false == bIsGlobalWhisper )
					{
						switch( g_pMain->GetNowStateID() )
						{
						case CX2Main::XS_PVP_ROOM:
						case CX2Main::XS_DUNGEON_ROOM:
						case CX2Main::XS_PVP_GAME:
						case CX2Main::XS_DUNGEON_GAME:
						case CX2Main::XS_SQUARE_GAME:
						case CX2Main::XS_TRAINING_GAME:
						case CX2Main::XS_VILLAGE_MAP:
						case CX2Main::XS_PVP_LOBBY:
						case CX2Main::XS_BATTLE_FIELD:
						case CX2Main::XS_WEDDING_GAME:
							{
								if ( NULL != g_pData->GetRelationshipManager() &&
									 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )								
								Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_RELATIONSHIP, eRoomChatType, g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_iRelationTargetUserUid, wstrChatMsg );
							}
							break;
						}
					}
					else
					{
						Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_WHISPER, eRoomChatType, iWhisperTargetUID, wstrChatMsg, 
							wstrWhisperTargetNickName.c_str() );
					}
				}

#endif // ADDED_RELATIONSHIP_SYSTEM

			//{{ kimhc // 2010.3.10 //	채팅창 개편
		#ifdef	CHAT_WINDOW_IMPROV
				if ( bIsEmotion == false )
					PushMsgToHistory( wstrChatMsg );

				UnHideChatLog( GetChatWindowMode() );
		#endif	CHAT_WINDOW_IMPROV
			//}} kimhc // 2010.3.10 //	채팅창 개편
			}
			
			ClearShortCutKeyProcess();			

			if( g_pTFieldGame != NULL && g_pTFieldGame->GetJoinNpc() == true && GetChatEditBox() == true)
			{
				UnHideChatEditBox();
			}

			DialogLog( L"ChatEnterEnd" );

			return true;
		}
		break;

	case PUM_CHAT_MODE_ALL:
		{
			ChangeChatWindowAndIMEEditBox( CWM_TOTAL );
			return true;
		} 
		break;

	case PUM_CHAT_MODE_PARTY:
		{
			ChangeChatWindowAndIMEEditBox( CWM_PARTY );
			return true;
		}
		break;

	case PUM_CHAT_MODE_GUILD:
		{
			//{{ 허상형 : [2009/9/16] //	길드 채팅 모드 변경
#ifdef	GUILD_MANAGEMENT
			ChangeChatWindowAndIMEEditBox( CWM_GUILD );
			return true;
#endif	//	GUILD_MANAGEMENT
			//}} 허상형 : [2009/9/16] //	길드 채팅 모드 변경

		}
		break;


	case PUM_CHAT_MODE_PERSONAL:
		{
			ChangeChatWindowAndIMEEditBox( CWM_PERSONAL );
			return true;
		} break;

#ifdef ADDED_RELATIONSHIP_SYSTEM

	case PUM_CHAT_MODE_RELATION :
		{
			if ( NULL != g_pData->GetRelationshipManager() && 
				 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() ) 
			{
				switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
				{
					case SEnum::RT_SOLO :
						ChangeChatWindowAndIMEEditBox( CWM_TOTAL );
						break;

					case SEnum::RT_COUPLE :
						ChangeChatWindowAndIMEEditBox( CWM_RELATION_COUPLE );
						break;
					
					case SEnum::RT_MARRIED :				
						ChangeChatWindowAndIMEEditBox( CWM_RELATION_MARRIED );
						break;
				}
			}
			return true;
		} break;

#endif // ADDED_RELATIONSHIP_SYSTEM
	case PUM_CHAT_WINDOW_OPEN_OR_CLOSE:
		{

			if( NULL != m_pDLGChatWindow )
			{
				switch( g_pMain->GetNowStateID() )
				{
				case CX2Main::XS_DUNGEON_GAME:
				case CX2Main::XS_PVP_GAME:
				case CX2Main::XS_TRAINING_GAME:
				case CX2Main::XS_BATTLE_FIELD:
					{
#if 0 
						CKTDGUIStatic* pStatic_BG = NULL; 
						if( true == m_bChatWindowEnlarged )
						{
							pStatic_BG = (CKTDGUIStatic*) m_pDLGChatWindow->GetControl( L"Chat_Box_Large" );
						}
						else
						{
							pStatic_BG = (CKTDGUIStatic*) m_pDLGChatWindow->GetControl( L"Chat_Box" );
						}

						if( NULL != pStatic_BG && false == pStatic_BG->GetShow() )
						{
							UnHideChatWindowFrame();
						}
						else
						{
							HideChatWindowFrame();
						}
#endif
					} break;

				default:
					{
						if( false == m_pDLGChatWindow->GetEnable() || false == m_pDLGChatWindow->GetShow() )
						{
							OpenChatWindow();
							//UnHideChatWindowFrame();
							//HideChatWindowFrame();
							UnHideChatWindowFrame();
							UnHideChatEditBox();

							m_bOpenChatWindowButton = true;							
						}
						else
						{
							CloseChatWindow();
							HideChatWindowFrame();
							HideChatEditBox();

							if(m_pDLGChatWindowHelper != NULL)
								m_pDLGChatWindowHelper->SetShowEnable(false, false);
							if(m_pDLGEmotionList != NULL)
								m_pDLGEmotionList->SetShowEnable(false, false);

							m_bOpenChatWindowButton = false;
						}
					} break;
				}
			}
			return true;
		} 
		break;

	case PUM_CHAT_WINDOW_SCROLL_TO_LATEST:
		{
			if( NULL != m_pDLGChatWindow )
			{

				CKTDGUIListBox* pListBox = GetCurrentChatbox();

				CKTDGUIListBox* pListBox_All = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_All" );
				CKTDGUIListBox* pListBox_Party = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Party" );
				CKTDGUIListBox* pListBox_Guild = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Guild" );
				CKTDGUIListBox* pListBox_Personal = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Personal" );
				CKTDGUIListBox* pListBox_Megaphone = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Megaphone" );

#ifdef ADDED_RELATIONSHIP_SYSTEM
				CKTDGUIListBox* pListBox_Relation = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Relation" );
#endif // ADDED_RELATIONSHIP_SYSTEM
				
				//{{ 허상형 : [2009/7/24] //	스크롤바 최하단 버튼을 눌렀을때 메시지가 보이도록 수정

				//	20.0f -> 50.0f
				const float MAGIC_CHAT_SHOW_TIME = 50.0f;

				if( NULL != pListBox_All )
				{
					pListBox_All->SetScrollBarEndPos();
					pListBox_All->SetShowAllItems( true );
					pListBox_All->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
				}

				if( NULL != pListBox_Party )
				{
					pListBox_Party->SetScrollBarEndPos();
					pListBox_Party->SetShowAllItems( true );
					pListBox_Party->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
				}

				if( NULL != pListBox_Guild )
				{
					pListBox_Guild->SetScrollBarEndPos();
					pListBox_Guild->SetShowAllItems( true );
					pListBox_Guild->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
				}

				if( NULL != pListBox_Personal )
				{
					pListBox_Personal->SetScrollBarEndPos();
					pListBox_Personal->SetShowAllItems( true );
					pListBox_Personal->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
				}


#ifdef ADDED_RELATIONSHIP_SYSTEM

				if( NULL != pListBox_Relation )
				{
					pListBox_Relation->SetScrollBarEndPos();
					pListBox_Relation->SetShowAllItems( true );
					pListBox_Relation->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
				}

#endif // ADDED_RELATIONSHIP_SYSTEM


				if( NULL != pListBox_Megaphone )
				{
					pListBox_Megaphone->SetScrollBarEndPos();
					pListBox_Megaphone->SetShowAllItems( true );
					//	메가폰은 계속 보이게
					pListBox_Personal->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
				}
				//}} 허상형 : [2009/7/24] //		스크롤바 최하단 버튼을 눌렀을때 메시지가 보이도록 수정

			}
			return true;
		} break;

	case PUM_CHAT_HELP:
		{
			// 도움말 창
			if( NULL != m_pDLGChatWindowHelper )
			{
				bool bShow = m_pDLGChatWindowHelper->GetShow();
				m_pDLGChatWindowHelper->SetShowEnable( !bShow, !bShow );
				
#ifdef ADDED_RELATIONSHIP_SYSTEM
				if ( TRUE == m_pDLGChatWindowHelper->GetShow() )
				{
					if ( NULL != g_pData->GetRelationshipManager() &&
				  	     NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
					{
						CKTDGUIButton * pRelationButton = static_cast <CKTDGUIButton*> ( m_pDLGChatWindowHelper->GetControl(L"RelationChatCouple") );
						CKTDGUIStatic * pStatic_RelationType = static_cast <CKTDGUIStatic*> ( m_pDLGChatWindowHelper->GetControl( L"Relation_Talk_Couple") );

						switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
						{
							case SEnum::RT_SOLO :
							{
								pRelationButton->SetShowEnable( false, false );
								pStatic_RelationType->SetShowEnable( false, false );
						

							
								D3DXVECTOR2 offset = D3DXVECTOR2 ( 0.f, 0.f );
								

								CKTDGUIStatic * pStatic_HelperBack = static_cast <CKTDGUIStatic*> ( m_pDLGChatWindowHelper->GetControl( L"g_pStaticChat_Bg") );
								pStatic_HelperBack->SetOffsetPos ( offset );
								if ( NULL != pStatic_HelperBack->GetPicture(0) )
								{
									pStatic_HelperBack->GetPicture(0)->SetSizeY(141-16);
								}
								
								offset.y -= 16.f;

								CKTDGUIButton * pHelpButton = static_cast <CKTDGUIButton*> ( m_pDLGChatWindowHelper->GetControl(L"Help100") );
								pHelpButton->SetOffsetPos( offset );
								CKTDGUIStatic * pStatic_Help = static_cast <CKTDGUIStatic*> ( m_pDLGChatWindowHelper->GetControl( L"Help") );
								pStatic_Help->SetOffsetPos( offset );

								CKTDGUIButton * pHelpEmotion = static_cast <CKTDGUIButton*> ( m_pDLGChatWindowHelper->GetControl(L"Emotion100") );
								pHelpEmotion->SetOffsetPos( offset );
								CKTDGUIStatic * pStatic_Emotion = static_cast <CKTDGUIStatic*> ( m_pDLGChatWindowHelper->GetControl( L"Emotion") );
								pStatic_Emotion->SetOffsetPos( offset );

							}
							break;
						
							case SEnum::RT_COUPLE :
							{
								pRelationButton->SetShowEnable( true, true );
								pStatic_RelationType->SetShowEnable( true, true );
								pStatic_RelationType->GetString(0)->msg = GET_STRING ( STR_ID_24581 );



								D3DXVECTOR2 offset = D3DXVECTOR2 ( 0.f, 0.f );
								
								CKTDGUIStatic * pStatic_HelperBack = static_cast <CKTDGUIStatic*> ( m_pDLGChatWindowHelper->GetControl( L"g_pStaticChat_Bg") );
								pStatic_HelperBack->SetOffsetPos ( offset );
								if ( NULL != pStatic_HelperBack->GetPicture(0) )
								{
									pStatic_HelperBack->GetPicture(0)->SetSizeY(141);
								}

								CKTDGUIButton * pHelpButton = static_cast <CKTDGUIButton*> ( m_pDLGChatWindowHelper->GetControl(L"Help100") );
								pHelpButton->SetOffsetPos( offset );
								CKTDGUIStatic * pStatic_Help = static_cast <CKTDGUIStatic*> ( m_pDLGChatWindowHelper->GetControl( L"Help") );
								pStatic_Help->SetOffsetPos( offset );

								CKTDGUIButton * pHelpEmotion = static_cast <CKTDGUIButton*> ( m_pDLGChatWindowHelper->GetControl(L"Emotion100") );
								pHelpEmotion->SetOffsetPos( offset );
								CKTDGUIStatic * pStatic_Emotion = static_cast <CKTDGUIStatic*> ( m_pDLGChatWindowHelper->GetControl( L"Emotion") );
								pStatic_Emotion->SetOffsetPos( offset );
							}
							break;

							case SEnum::RT_MARRIED :
							{
							
								pRelationButton->SetShowEnable( true, true );
								pStatic_RelationType->SetShowEnable( true, true );
								pStatic_RelationType->GetString(0)->msg = GET_STRING ( STR_ID_24582 );


								D3DXVECTOR2 offset = D3DXVECTOR2 ( 0.f, 0.f );
								
								CKTDGUIStatic * pStatic_HelperBack = static_cast <CKTDGUIStatic*> ( m_pDLGChatWindowHelper->GetControl( L"g_pStaticChat_Bg") );
								pStatic_HelperBack->SetOffsetPos ( offset );
								if ( NULL != pStatic_HelperBack->GetPicture(0) )
								{
									pStatic_HelperBack->GetPicture(0)->SetSizeY(141);
								}

								CKTDGUIButton * pHelpButton = static_cast <CKTDGUIButton*> ( m_pDLGChatWindowHelper->GetControl(L"Help100") );
								pHelpButton->SetOffsetPos( offset );
								CKTDGUIStatic * pStatic_Help = static_cast <CKTDGUIStatic*> ( m_pDLGChatWindowHelper->GetControl( L"Help") );
								pStatic_Help->SetOffsetPos( offset );

								CKTDGUIButton * pHelpEmotion = static_cast <CKTDGUIButton*> ( m_pDLGChatWindowHelper->GetControl(L"Emotion100") );
								pHelpEmotion->SetOffsetPos( offset );
								CKTDGUIStatic * pStatic_Emotion = static_cast <CKTDGUIStatic*> ( m_pDLGChatWindowHelper->GetControl( L"Emotion") );
								pStatic_Emotion->SetOffsetPos( offset );
							}
							break;
						}
					}
				}
#endif // ADDED_RELATIONSHIP_SYSTEM


			}
			return true;

		} break;

		//{{ kimhc // 2010.3.11 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
	case PUM_HELPER_ALL:
		UnHideChatEditBox();
		ChangeIMEEditBox( CWM_TOTAL );
		SetFocusChatEditBox( true );
		return true;
	case PUM_HELPER_PARTY:
		UnHideChatEditBox();
		ChangeIMEEditBox( CWM_PARTY );
		SetFocusChatEditBox( true );
		return true;

		//{{ 허상형 : [2009/11/2] //	길드대화 버튼 클릭 이벤트
	case PUM_HELPER_GUILD:
		{
#ifdef GUILD_MANAGEMENT
			UnHideChatEditBox();
			ChangeIMEEditBox( CWM_GUILD );
			SetFocusChatEditBox( true );
#endif	//	GUILD_MANAGEMENT
		}
		break;
		//}} 허상형 : [2009/11/2] //	길드대화 버튼 클릭 이벤트

	case PUM_HELPER_WHISPER:
		UnHideChatEditBox();
		ChangeIMEEditBox( CWM_PERSONAL );
		SetFocusChatEditBox( true );
		return true;

	case PUM_HELPER_REPLY:	
		{
			UnHideChatEditBox();

			wstring wstrText = L"";
			CKTDGUIIMEEditBox* pEdit_Chat = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );

			if(pEdit_Chat != NULL)
			{
#ifdef GLOBAL_CHAT_FUNCTION
				wstrText = GET_STRING(STR_ID_14331);
				wstrText += L" ";
#else GLOBAL_CHAT_FUNCTION
				wstrText = L"/w ";
#endif GLOBAL_CHAT_FUNCTION
				wstrText += m_wstrLastWhiperNickName;
				wstrText += L" ";
				pEdit_Chat->SetText( wstrText.c_str() );
			}			

			SetFocusChatEditBox( true );

			return true;
		}
		break;

#ifdef ADDED_RELATIONSHIP_SYSTEM	
	case PUM_HELPER_RELATION:
		UnHideChatEditBox();

		if ( NULL != g_pData->GetRelationshipManager() && 
			NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() ) 
		{
			switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
			{
			case SEnum::RT_SOLO :
				ChangeIMEEditBox( CWM_TOTAL );
				break;

			case SEnum::RT_COUPLE :
				ChangeIMEEditBox( CWM_RELATION_COUPLE );
				break;

			case SEnum::RT_MARRIED :				
				ChangeIMEEditBox( CWM_RELATION_MARRIED );
				break;
			}
		}

		SetFocusChatEditBox( true );
		return true;
#endif // ADDED_RELATIONSHIP_SYSTEM

#else	CHAT_WINDOW_IMPROV
	case PUM_HELPER_ALL:
		ChangeChatWindowAndIMEEditBox( CWM_TOTAL );
		return true;
	case PUM_HELPER_PARTY:
		ChangeChatWindowAndIMEEditBox( CWM_PARTY );
		return true;

		//{{ 허상형 : [2009/11/2] //	길드대화 버튼 클릭 이벤트
	case PUM_HELPER_GUILD:
		{
#ifdef GUILD_MANAGEMENT
			ChangeChatWindowAndIMEEditBox( CWM_GUILD );
#endif	//	GUILD_MANAGEMENT
		}
		break;
		//}} 허상형 : [2009/11/2] //	길드대화 버튼 클릭 이벤트

	case PUM_HELPER_WHISPER:
		ChangeChatWindowAndIMEEditBox( CWM_PERSONAL );
		return true;

	case PUM_HELPER_REPLY:	
		{
			wstring wstrText = L"";
			CKTDGUIIMEEditBox* pEdit_Chat = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );
			
			if(pEdit_Chat != NULL)
			{
				ChangeChatWindowAndIMEEditBox( CWM_PERSONAL );

#ifdef GLOBAL_CHAT_FUNCTION
				wstrText = GET_STRING(STR_ID_14331);
				wstrText += L" ";
#else GLOBAL_CHAT_FUNCTION
				wstrText = L"/w ";
#endif GLOBAL_CHAT_FUNCTION
				wstrText += m_wstrLastWhiperNickName;
				wstrText += L" ";
				pEdit_Chat->SetText( wstrText.c_str() );
			}			

			return true;
		}
		break;
#endif	CHAT_WINDOW_IMPROV
		//}} kimhc // 2010.3.11 //	채팅창 개편

	case PUM_HELPER_EMOTION:
		{
			// 이모션 리스트
			bool bShow = m_pDLGEmotionList->GetShow();
			m_pDLGEmotionList->SetShowEnable( !bShow, !bShow );
			return true;			
		}
		break;
	case PUM_HELPER_EMOTION_SELECT:
		{
			// 이모션 선택
			if(m_pDLGEmotionList != NULL && m_pDLGEmotionList->GetShow() == true)
			{
				CKTDGUIListBox *pListbox = (CKTDGUIListBox*)m_pDLGEmotionList->GetControl(L"EmotionList");
				if(pListbox != NULL)
				{
					CKTDGUIListBox::ListBoxItem *pItem = pListbox->GetSelectedItem();

					if(pItem != NULL)
					{
						SendEmotionId(pItem->strText);
					}
				}		

				m_pDLGEmotionList->SetShowEnable(false, false);
			}			
		}
		break;
	case PUM_HELP:
		{
			for( UINT i=0; i<m_vecChatHelpString.size(); i++ )
			{
				AddChatLog( m_vecChatHelpString[i].c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
			}
		}
		break;
	case PUM_POPUP_OPTION_MENU_ALL:
		{
			ResetChatTabPopup(CX2ChatBox::CWM_TOTAL);
		}
		break; 
	case PUM_POPUP_OPTION_MENU_PARTY:
		{
			ResetChatTabPopup(CX2ChatBox::CWM_PARTY);
		}
		break; 
	case PUM_POPUP_OPTION_MENU_GUILD:
		{
			ResetChatTabPopup(CX2ChatBox::CWM_GUILD);
		}
		break; 
	case PUM_POPUP_OPTION_MENU_WHISPER:
		{
			ResetChatTabPopup(CX2ChatBox::CWM_PERSONAL);
		}
		break; 

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case PUM_POPUP_OPTION_MENU_RELATION:
		{
			if ( NULL != g_pData->GetRelationshipManager() && 
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() ) 
			{
				switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
				{
				case SEnum::RT_SOLO :
					ResetChatTabPopup( CX2ChatBox::CWM_TOTAL );
					break;

				case SEnum::RT_COUPLE :
					ResetChatTabPopup( CX2ChatBox::CWM_RELATION_COUPLE );
					break;

				case SEnum::RT_MARRIED :				
					ResetChatTabPopup( CX2ChatBox::CWM_RELATION_MARRIED );
					break;
				}
			}
			
		}
		break; 
#endif // ADDED_RELATIONSHIP_SYSTEM
	case PUM_POPUP_OPTION_MENU_CLOSE:
		{
			if( NULL != m_pDLGTabMenu )
				m_pDLGTabMenu->SetShowEnable(false,false);
		}
		break;
	case PUM_OPTION_CHECKED_EXP:
		{
			switch( m_ePopupWindowMode )
			{
			case CWM_TOTAL:
				{
					SetCheckBoxData(COI_TOTAL_EXP);
				} break;
			case CWM_PARTY:
				{
					SetCheckBoxData(COI_PARTY_EXP);
				} break;
			case CWM_GUILD:
				{
					SetCheckBoxData(COI_GUILD_EXP);
				} break;
			case CWM_PERSONAL:
				{
					SetCheckBoxData(COI_WHISPER_EXP);
				} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CWM_RELATION_COUPLE:
			case CWM_RELATION_MARRIED :
				{
					SetCheckBoxData(COI_RELATION_EXP);
				} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
			default:
				break;
			}
		}
		break;
	case PUM_OPTION_CHECKED_ED:
		{
			switch( m_ePopupWindowMode )
			{
			case CWM_TOTAL:
				{
					SetCheckBoxData(COI_TOTAL_ED);
				} break;
			case CWM_PARTY:
				{
					SetCheckBoxData(COI_PARTY_ED);
				} break;
			case CWM_GUILD:
				{
					SetCheckBoxData(COI_GUILD_ED);
				} break;
			case CWM_PERSONAL:
				{
					SetCheckBoxData(COI_WHISPER_ED);
				} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CWM_RELATION_MARRIED:
			case CWM_RELATION_COUPLE:
				{
					SetCheckBoxData(COI_RELATION_ED);
				} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
			default:
				break;
			}

		}
		break;
	case PUM_OPTION_CHECKED_SYSTEM:
		{
			switch( m_ePopupWindowMode )
			{
			case CWM_TOTAL:
				{
					SetCheckBoxData(COI_TOTAL_SYSTEM);
				} break;
			case CWM_PARTY:
				{
					SetCheckBoxData(COI_PARTY_SYSTEM);
				} break;
			case CWM_GUILD:
				{
					SetCheckBoxData(COI_GUILD_SYSTEM);
				} break;
			case CWM_PERSONAL:
				{
					SetCheckBoxData(COI_WHISPER_SYSTEM);
				} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CWM_RELATION_COUPLE :
			case CWM_RELATION_MARRIED :
				{
					SetCheckBoxData(COI_RELATION_SYSTEM);
				} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
			default:
				break;
			}

		}
		break;
	case PUM_OPTION_CHECKED_NORMAL:
		{
			switch( m_ePopupWindowMode )
			{
			case CWM_TOTAL:
				{
					SetCheckBoxData(COI_TOTAL_NORMAL);
				} break;
			default:
				break;
			}

		}
		break;
	case PUM_OPTION_CHECKED_PARTY:
		{
			switch( m_ePopupWindowMode )
			{
			case CWM_TOTAL:
				{
					SetCheckBoxData(COI_TOTAL_PARTY);
				} break;
			case CWM_PARTY:
				{
					SetCheckBoxData(COI_PARTY_PARTY);
				} break;
			default:
				break;
			}

		}
		break;
	case PUM_OPTION_CHECKED_GUILD:
		{
			switch( m_ePopupWindowMode )
			{
			case CWM_TOTAL:
				{
					SetCheckBoxData(COI_TOTAL_GUILD);
				} break;
			case CWM_GUILD:
				{
					SetCheckBoxData(COI_GUILD_GUILD);
				} break;
			default:
				break;
			}

		}
		break;
	case PUM_OPTION_CHECKED_WHISPER:
		{
			switch( m_ePopupWindowMode )
			{
			case CWM_TOTAL:
				{
					SetCheckBoxData(COI_TOTAL_WHISPER);
				} break;
			case CWM_PARTY:
				{
					SetCheckBoxData(COI_PARTY_WHISPER);
				} break;
			case CWM_GUILD:
				{
					SetCheckBoxData(COI_GUILD_WHISPER);
				} break;
			case CWM_PERSONAL:
				{
					SetCheckBoxData(COI_WHISPER_WHISPER);
				} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CWM_RELATION_COUPLE:
			case CWM_RELATION_MARRIED:
				{
					SetCheckBoxData(COI_RELATION_WHISPER);
				} break;
#endif // 	ADDED_RELATIONSHIP_SYSTEM
			default:
				break;
			}
		}
		break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case PUM_OPTION_CHECKED_RELATION:
		{
			switch( m_ePopupWindowMode )
			{
			case CWM_TOTAL:
				{
					SetCheckBoxData(COI_TOTAL_RELATION);
				} break;
			case CWM_PERSONAL:
				{
					SetCheckBoxData(COI_WHISPER_RELATION);
				} break;
			case CWM_RELATION_MARRIED :
			case CWM_RELATION_COUPLE :
				{
					SetCheckBoxData(COI_RELATION_RELATION);
				} break;
			default:
				break;
			}
		}
		break;

#endif // ADDED_RELATIONSHIP_SYSTEM
		//{{ 허상형 : [2009/7/14] // 나소드 메가폰 관련 이벤트 프로시저
	case NSUM_INSERT_MESSAGE_CLOSE:
		{
			return ShowNasodMessageDlg( false );
		}
		break;
	case NSUM_INSERT_MESSAGE_OK:
		{
			if( m_pDLGNasodScopeMessage != NULL)
			{
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGNasodScopeMessage->GetControl( L"IMEEditBoxMessage" );
				if(pIMEEditBox != NULL)
				{
					if(m_pSelectedItemSlot != NULL || m_iLastUsedMegaphoneTID != -1) // oasis907 : 김상윤 [2010.3.29] //  수정
					{
						//	메시지 입력이 없을 경우
						if( wcscmp(pIMEEditBox->GetText(), L"") == 0 )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_3742 ) , g_pMain->GetNowState() );
							return true;
						}

						//	금지어가 포함 되어 있을 경우
						if( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_CHAT, pIMEEditBox->GetText() ) == false )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_3743 ) , g_pMain->GetNowState() );
							return true;
						}
						
						//	메시지가 18자 이상일 경우 잘라내고 뒤에 ...을 붙인다.
						wstring wstrMessage = pIMEEditBox->GetText();
						wstring wstrResult;

						if( wstrMessage.length() > 18 )
						{
							wstrResult = wstrMessage.substr(0, 18) + L"...";
						}
						else
						{
							wstrResult = wstrMessage;
						}
						const CX2Item::ItemTemplet* pkItemTemplet = NULL;
						//{{ oasis907 : 김상윤 [2010.3.29] // 
						if(m_iLastUsedMegaphoneTID != -1)
						{
							pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet(m_iLastUsedMegaphoneTID);
						}
						//}}
						else
						{
							pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pSelectedItemSlot->GetItemTID() );
						}
#ifdef NASOD_SCOPE_ID_SAVE 
						// 이런 경우가 없을 거 같긴 하지만 만약을 위해서 만약 NULL이 들어온다면 다이얼로그를 꺼버리자.
						if( NULL == pkItemTemplet )
						{
							ShowNasodMessageDlg( false );
						}
#endif //NASOD_SCOPE_ID_SAVE
						m_pDLGNasodScopeMessageYesNo = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
                            GET_REPLACED_STRING( ( STR_ID_3740, "LS", wstrResult, pkItemTemplet->GetFullName_() ) ),
                             NSUM_USE_ITEM_OK, g_pMain->GetNowState(), NSUM_USE_ITEM_CANCEL );

						return true;
					}
				}
			}

			return true;
		}
		break;
	case NSUM_USE_ITEM_CANCEL:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			m_pDLGNasodScopeMessageYesNo = NULL;
			return true;
		}
		break;
	case NSUM_USE_ITEM_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			m_pDLGNasodScopeMessageYesNo = NULL;
			Handler_EGS_USE_MEGAPHONE_REQ();
			return true;
		}
		break;
	case NSUM_CHAT_MODE_MEGAPHONE:
		{
			ChangeChatWindowAndIMEEditBox( CWM_MEGAPHONE );
			return true;
		} break;
		//	메가폰에 마우스 좌, 우클릭, 더블클릭, 귓속말 선택 프로시저
	case NSUM_CHAT_SELECT_MESSAGE:
	case NSUM_CHAT_OPEN_MENU:
		{
			if( NULL != m_pDLGNasodScopeUserMenu )
				m_pDLGNasodScopeUserMenu->SetShowEnable(false, false);

			ClearSelectChatListBox();
			//우클릭일 때만 팝업 동작
			bool bPopup = (wParam == NSUM_CHAT_OPEN_MENU );
			PopupNasodScopeUserMenu(bPopup);

			return true;
		} break;
	case NSUM_CHAT_CLOSE_MENU:
		{
			m_pDLGNasodScopeUserMenu->SetShowEnable(false, false);

			return true;
		} break;;
	case NSUM_MENU_FRIEND:
		{
#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP

			m_pDLGNasodScopeUserMenu->SetShowEnable(false, false);

			// 체험 아이디 제한 
			if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
				return true;
			}
			if( m_wstrPickedUserNickName != g_pData->GetMyUser()->GetSelectUnit()->GetNickName() )
			{
#ifdef NEW_MESSENGER
				wstring addFriendMessage = GET_STRING( STR_ID_74 );
				g_pData->GetMessenger()->Handler_EGS_REQUEST_FRIEND_REQ( m_wstrPickedUserNickName.c_str(), addFriendMessage );
#else
				g_pData->GetMessenger()->Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ( m_wstrPickedUserNickName.c_str() );
#endif
				g_pData->GetMessenger()->SetOpen( true );
			}
#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP

			return true;	
		} break;

		//{{ 허상형 : [2009/7/22] //
	case NSUM_MENU_CLEAR:
		{
			CKTDGUIListBox* pListBox;

			pListBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_All" );
			if( pListBox != NULL ) pListBox->ClearSelect();
			pListBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Party" );
			if( pListBox != NULL ) pListBox->ClearSelect();
			pListBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Personal" );
			if( pListBox != NULL ) pListBox->ClearSelect();
			pListBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Guild" );
			if( pListBox != NULL ) pListBox->ClearSelect();
			pListBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Megaphone" );
			if( pListBox != NULL ) pListBox->ClearSelect();

#ifdef ADDED_RELATIONSHIP_SYSTEM
			pListBox = static_cast <CKTDGUIListBox*> ( m_pDLGChatWindow->GetControl( L"ListBox_Relation" ) );
			if( pListBox != NULL ) pListBox->ClearSelect();
#endif // ADDED_RELATIONSHIP_SYSTEM

		} break;
		//}} 허상형 : [2009/7/22] //
		//	더블 클릭일 경우 귓말 모드로 전환 )
	case NSUM_MENU_WHISPER:
		{
			//	기존 창 닫기
			m_pDLGNasodScopeUserMenu->SetShowEnable(false, false);

			g_pChatBox->OnOpenWhisperChatMode();

			return true;
		} break;
		//}} 허상형 : [2009/7/14] // 나소드 메가폰 관련 이벤트 프로시저
	}

	return false;
}

bool CX2ChatBox::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();
	switch( wParam )
	{

	case EGS_CHAT_ACK:
		{
			return Handler_EGS_CHAT_ACK( hWnd, uMsg, wParam, lParam );
		} 
		break;

	case EGS_CHAT_NOT:
		{
			return Handler_EGS_CHAT_NOT( hWnd, uMsg, wParam, lParam );
		} 
		break;

	case EGS_NEW_BLACKLIST_USER_ACK:
		{
			return Handler_EGS_NEW_BLACKLIST_USER_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_DEL_BLACKLIST_USER_ACK:
		{
			return Handler_EGS_DEL_BLACKLIST_USER_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

		//{{ 허상형 : [2009/7/15] // 나소드 메가폰 핸들러
	case EGS_USE_MEGAPHONE_ACK:
		{
			return Handler_EGS_USE_MEGAPHONE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_MEGAPHONE_MESSAGE_NOT:
		{
			return Handler_EGS_MEGAPHONE_MESSAGE_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
		//}} 허상형 : [2009/7/15] // 나소드 메가폰 핸들러
	case EGS_CHAT_OPTION_INFO_WRITE_ACK:
		{
			return Handler_EGS_CHAT_OPTION_INFO_WRITE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
		//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
	case EGS_ANTI_ADDICTION_NOT:
		{
			return Handler_EGS_ANTI_ADDICTION_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_ANTI_ADDICTION_SYSTEM
		//}}
	}

	return false;
}


bool CX2ChatBox::Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CHAT_PACKET_TYPE eChatPacketType, CX2Room::ROOM_CHAT_TYPE eChatType, 
										   UidType iTargetUnitUID, wstring& wstrMsg, const WCHAR* wszTargetUnitName /*= NULL*/ )
{

	if( wstrMsg.empty() )
	{
		return false;
	}

	if( KEGS_CHAT_REQ::CPT_WHISPER == eChatPacketType )
	{
		if( NULL == wszTargetUnitName )
			return false;
	}
	else
	{
		if( CX2Room::RCT_WHISPER == eChatType )
		{
			if( -1 == iTargetUnitUID ||
				iTargetUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			{
				return false;
			}
		}
	}

	wstrMsg = wstrMsg.substr( 0, MAX_ROOM_CHAT_STR_LEN );

	KEGS_CHAT_REQ kPacket;
	kPacket.m_cChatPacketType = (char) eChatPacketType;
	kPacket.m_cRoomChatType = (char) eChatType;
	kPacket.m_ToUnitUID		= iTargetUnitUID;
	kPacket.m_wstrMsg		= wstrMsg;

	if( KEGS_CHAT_REQ::CPT_WHISPER == eChatPacketType )
	{
		if( NULL != wszTargetUnitName )
		{
			kPacket.m_wstrRecieverUnitNickName = wszTargetUnitName;
		}
		else
		{
			return false;
		}
	}

#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( KEGS_CHAT_REQ::CPT_RELATIONSHIP == eChatPacketType )
	{
		if ( NULL != g_pData->GetRelationshipManager() &&
			 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
		{
			kPacket.m_wstrRecieverUnitNickName = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname;
		}
		else
		{
			return false;
		}

	}
#endif // ADDED_RELATIONSHIP_SYSTEM



#ifdef NEW_MESSENGER
	if( KEGS_CHAT_REQ::CPT_MESSENGER == eChatPacketType )
	{
		if( NULL != wszTargetUnitName )
		{
			kPacket.m_wstrRecieverUnitNickName = wszTargetUnitName;
		}
		else
		{
			return false;
		}
	}
#endif

	if ( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
	{
		kPacket.m_wstrMsg = g_pMain->GetStringFilter()->FilteringChatString( kPacket.m_wstrMsg.c_str(), L'♡' );
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_CHAT_REQ, kPacket );

#ifdef SERV_PET_SYSTEM
	if( g_pData != NULL && g_pData->GetPetManager() != NULL )
	{
		// 내가 dungeon ready상태이면 펫 명령을 막는다.		
		//if( g_pData->GetPartyManager()->DoIHaveParty() == true )
		//{
		//	bool bMyReady = false;
		//	UidType iUnitUid = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
		//	CX2PartyManager::PartyMemberData* pMemberData = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( iUnitUid );
		//	if( NULL != pMemberData )
		//	{
		//		bMyReady = pMemberData->m_bGameReady;
		//	}
		//	if( bMyReady == true )
		//		return true;
		//}		

		CX2PET *pPet = g_pData->GetPetManager()->GetMyPet();
		if( pPet != NULL && g_pData->GetPetManager()->GetPetStatus( pPet->GetPetInfo() ) > 0 )
		{
			CX2PET::PET_ACTION_COMMAND eActionCommand = g_pData->GetPetManager()->GetPetActionCommand( wstrMsg );
#ifdef _IN_HOUSE_
			if( eActionCommand == CX2PET::PAC_ATTACK )
			{
				pPet->StateChange( pPet->m_AttackState );
				return true;
			}
			else if( eActionCommand == CX2PET::PAC_CHEER )
			{
				pPet->StateChange( pPet->m_CheerState );
				return true;
			}
#endif
			if( eActionCommand != CX2PET::PAC_NONE )
				g_pData->GetPetManager()->SendPetActionCommand( eActionCommand );
		}		
	}
#endif

	return true;
}

bool CX2ChatBox::Handler_EGS_CHAT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHAT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );


	if ( kEvent.m_iOK == NetError::ERR_CHAT_01 )
	{
		switch( GetChatWindowMode() )
		{

		case CWM_TOTAL:
			{
				AddChatLog( GET_STRING( STR_ID_58 ), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			}
			break;

		case CWM_PARTY:
			{
				AddChatLog( GET_STRING( STR_ID_58 ), KEGS_CHAT_REQ::CPT_PARTY, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			}
			break;
		}

		return true;
	}
#ifdef SERV_INTEGRATION
	//{{ oasis907 : 김상윤 [2010.5.19] // 대전 던전 서버군 통합
	if ( kEvent.m_iOK == NetError::ERR_CHAT_04)
	{
		wstring wstrServerName = L"";
#ifdef SERVER_GROUP_UI_ADVANCED // 2012.07.09 lygan_조성욱 // 서버군 확장 용의하게 추가
		wstrServerName = g_pInstanceData->GetServerGroupName();
#else //SERVER_GROUP_UI_ADVANCED
		switch(g_pInstanceData->GetServerGroupID())
		{
		case SGI_SOLES:
			wstrServerName = GET_STRING( STR_ID_4836 );
			break;
		case SGI_GAIA:
			wstrServerName = GET_STRING( STR_ID_4835 );
			break;
		default:
			wstrServerName = GET_STRING( STR_ID_5131 );
			break;
		}
#endif SERVER_GROUP_UI_ADVANCED
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_REPLACED_STRING( ( STR_ID_5132, "L", wstrServerName ) ), g_pMain->GetNowState() );
		return true;
	}
	//}}
#endif SERV_INTEGRATION

	return g_pMain->IsValidPacket( kEvent.m_iOK );
}


bool CX2ChatBox::Handler_EGS_CHAT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHAT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	DialogLog( L"ChatNotStart" );

	//wstring		wstrWhisperColor = L"#C2262FF";				// 22, 98, 255
	//D3DXCOLOR	coWhisperColor(0.08627f, 0.38431f, 1, 1);		// 22, 98, 255

#ifdef NEW_MESSENGER
	// 가장 먼저 메신저 채팅인지 확인해서 
	if( kEvent.m_cChatPacketType == KEGS_CHAT_REQ::CPT_MESSENGER )
	{
		if( g_pData != NULL && g_pData->GetMessenger() != NULL )
		{
			if( kEvent.m_SenderUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID())
			{
				// 내가 보낸 메시지인 경우 : 상대방(받는 사람)의 UID를 Session의 Key로 사용한다
				return g_pData->GetMessenger()->ReceiveChatMessage( kEvent.m_RecieverUnitUID, kEvent.m_wstrMsg, kEvent.m_wstrSenderUnitNickName );
			}
			else
			{
				// 상대가 보낸 메시지인 경우 : 보낸 사람의 UID를 Session의 Key로 사용한다
				return g_pData->GetMessenger()->ReceiveChatMessage( kEvent.m_SenderUnitUID, kEvent.m_wstrMsg, kEvent.m_wstrSenderUnitNickName );

			}
		}			
		else
			return false;		
	}
#endif

#ifdef UNIT_EMOTION
	bool bEmotionCommand = IsEmotionID(kEvent.m_wstrMsg);
#else
	bool bEmotionCommand = false;
#endif

	// fix!! 귓속말이 왔을때는 어떻게???
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			if( false == m_bOpenChatWindow && bEmotionCommand == false)
			{
				OpenChatWindow();
				//HideChatWindowFrame();
			}
		} break;
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_SQUARE_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_TRADE) == true )
			//if( true == ((CX2StateSquareGame*)g_pMain->GetNowState())->GetPersonalTrade()->GetEnable() ) // 개인거래 중이면
			{
				if( false == m_bOpenChatWindow && bEmotionCommand == false)
				{
					OpenChatWindow();
					//HideChatWindowFrame();
				}
			}
		} break;
	}

	bool bIsMyself = false;

	if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		if( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_SenderUnitUID )
		{
			bIsMyself = true;
		}
	}

	switch( kEvent.m_cChatPacketType )
	{
	case KEGS_CHAT_REQ::CPT_TOTAL:
		{
			wstring wstrSenderNickName = L"";
			wstring wstrReceiverNickName = L"";
			wstringstream wstrmTalkBoxMsg;

			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_DUNGEON_GAME:
			case CX2Main::XS_DUNGEON_ROOM:
			case CX2Main::XS_PVP_GAME:
			case CX2Main::XS_PVP_ROOM:
			case CX2Main::XS_BATTLE_FIELD:
				{
					if( NULL == g_pX2Room )
						return false;

					CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( kEvent.m_SenderUnitUID );
					if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
					{
						pSlotData = g_pX2Room->GetObserverSlotDataByUnitUID( kEvent.m_SenderUnitUID );
						if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
						{
							return false;
						}
					}
					wstrSenderNickName = pSlotData->m_pUnit->GetNickName();


					if( kEvent.m_cRoomChatType == CX2Room::RCT_WHISPER )
					{
						CX2Room::SlotData* pReceiverSlotData = g_pX2Room->GetSlotDataByUnitUID( kEvent.m_RecieverUnitUID );
						if( pReceiverSlotData == NULL || pReceiverSlotData->m_pUnit == NULL )
						{
							pReceiverSlotData = g_pX2Room->GetObserverSlotDataByUnitUID( kEvent.m_RecieverUnitUID );
							if( pReceiverSlotData == NULL || pReceiverSlotData->m_pUnit == NULL )
							{
								return false;
							}
						}

						wstrReceiverNickName = pReceiverSlotData->m_pUnit->GetNickName();
					}

				} break;

			case CX2Main::XS_VILLAGE_MAP:
				{
					if( NULL == g_pTFieldGame )
						return false;

					CX2SquareUnit* pCX2SquareUnit_Sender = g_pTFieldGame->GetSquareUnitByUID( kEvent.m_SenderUnitUID );
					if( NULL == pCX2SquareUnit_Sender || NULL == pCX2SquareUnit_Sender->GetUnit() )
						return false;
					else
						wstrSenderNickName = pCX2SquareUnit_Sender->GetNickName();

					if( kEvent.m_cRoomChatType == CX2Room::RCT_WHISPER )
					{
						CX2SquareUnit* pCX2SquareUnit_Receiver = g_pTFieldGame->GetSquareUnitByUID( kEvent.m_RecieverUnitUID );
						if( NULL == pCX2SquareUnit_Receiver || NULL == pCX2SquareUnit_Receiver->GetUnit() )
							return false;
						else
							wstrReceiverNickName = pCX2SquareUnit_Receiver->GetNickName();
					}

				} break;
			case CX2Main::XS_SQUARE_GAME:
				{
					if( NULL == g_pSquareGame )
						return false;

					CX2SquareUnit* pCX2SquareUnit_Sender = g_pSquareGame->GetSquareUnitByUID( kEvent.m_SenderUnitUID );
					if( NULL == pCX2SquareUnit_Sender || NULL == pCX2SquareUnit_Sender->GetUnit() )
						return false;
					else
						wstrSenderNickName = pCX2SquareUnit_Sender->GetNickName();

					if( kEvent.m_cRoomChatType == CX2Room::RCT_WHISPER )
					{
						CX2SquareUnit* pCX2SquareUnit_Receiver = g_pSquareGame->GetSquareUnitByUID( kEvent.m_RecieverUnitUID );
						if( NULL == pCX2SquareUnit_Receiver || NULL == pCX2SquareUnit_Receiver->GetUnit() )
							return false;
						else
							wstrReceiverNickName = pCX2SquareUnit_Receiver->GetNickName();
					}
				} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CX2Main::XS_WEDDING_GAME:
				{
					if( NULL == g_pWeddingGame )
						return false;

					CX2SquareUnit* pCX2SquareUnit_Sender = g_pWeddingGame->GetSquareUnitByUID( kEvent.m_SenderUnitUID );
					if( NULL == pCX2SquareUnit_Sender || NULL == pCX2SquareUnit_Sender->GetUnit() )
						return false;
					else
						wstrSenderNickName = pCX2SquareUnit_Sender->GetNickName();

					if( kEvent.m_cRoomChatType == CX2Room::RCT_WHISPER )
					{
						CX2SquareUnit* pCX2SquareUnit_Receiver = g_pWeddingGame->GetSquareUnitByUID( kEvent.m_RecieverUnitUID );
						if( NULL == pCX2SquareUnit_Receiver || NULL == pCX2SquareUnit_Receiver->GetUnit() )
							return false;
						else
							wstrReceiverNickName = pCX2SquareUnit_Receiver->GetNickName();
					}
				} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
			}

			if(wstrSenderNickName.empty() == true)
				return false;

			wstring wstrColor = L"";				
			D3DXCOLOR coTextColor(0, 0, 0, 1);		

			switch( kEvent.m_cRoomChatType )
			{
				//{{ kimhc // 2010-03-11 // 채팅창 개편
	#ifdef	CHAT_WINDOW_IMPROV
				// 대전 or 대전 룸에서 RCT_TEAM을 받은 경우 파티창에 표시 될수 있게 수정
			case CX2Room::RCT_ALL:
				{
					wstring chatContent = wstrSenderNickName;
					chatContent += L" : ";
					chatContent += kEvent.m_wstrMsg;
					wstrmTalkBoxMsg << kEvent.m_wstrMsg;
					if( bEmotionCommand == false )						
					{
#ifdef CHAT_LOG_TO_TXT
						AddChatLogToFile( chatContent.c_str() );
#endif // CHAT_LOG_TO_TXT
						AddChatLog( chatContent.c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(1,1,1,1), L"", false, bIsMyself );
					}

				} break;

			case CX2Room::RCT_TEAM:
				{
					wstring chatContent = wstrSenderNickName;
					chatContent += L" : ";
					chatContent += kEvent.m_wstrMsg;
					wstrmTalkBoxMsg << kEvent.m_wstrMsg;

					switch ( g_pMain->GetNowStateID() )
					{
					case CX2Main::XS_PVP_ROOM:
					case CX2Main::XS_PVP_GAME:
					case CX2Main::XS_PVP_RESULT:
						{
							wstring		wstrColor = L"#C42FFFF";						// 66, 255, 255
							D3DXCOLOR	coTextColor = D3DXCOLOR(0.258f, 1, 1, 1);		// 66, 255, 255

							if( bEmotionCommand == false )						
							{
#ifdef CHAT_LOG_TO_TXT
								AddChatLogToFile( chatContent.c_str() );
#endif // CHAT_LOG_TO_TXT
								AddChatLog( chatContent.c_str(), KEGS_CHAT_REQ::CPT_PARTY, coTextColor, wstrColor, false, bIsMyself );
							}
						}
						break;

					default:
						{
							if( bEmotionCommand == false )						
							{
#ifdef CHAT_LOG_TO_TXT
								AddChatLogToFile( chatContent.c_str() );
#endif // CHAT_LOG_TO_TXT
								AddChatLog( chatContent.c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(1,1,1,1), L"", false, bIsMyself );
							}
						}
						break;
					}
					
				} break;

	#else	CHAT_WINDOW_IMPROV
			case CX2Room::RCT_ALL:
			case CX2Room::RCT_TEAM:
				{

					wstring chatContent = wstrSenderNickName;
					chatContent += L" : ";
					chatContent += kEvent.m_wstrMsg;
					wstrmTalkBoxMsg << kEvent.m_wstrMsg;
					if( bEmotionCommand == false )						
						AddChatLog( chatContent.c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(1,1,1,1), L"", false, bIsMyself );
				} break;
	#endif	CHAT_WINDOW_IMPROV
				//}} kimhc // 2010-03-11 // 채티창 개편

			case CX2Room::RCT_WHISPER:
				{
					wstringstream wstrmChatContent;
#ifdef UPDATE_TO_STR_IN_WISPER
					wstrmChatContent << wstrSenderNickName << L" : (" << GET_STRING( STR_ID_28125 ) << " ";
#else
					wstrmChatContent << wstrSenderNickName << L" : (to ";
#endif UPDATE_TO_STR_IN_WISPER
					wstrmChatContent << wstrReceiverNickName << ") ";
					wstrmChatContent << kEvent.m_wstrMsg;

#ifdef UPDATE_TO_STR_IN_WISPER
					wstrmTalkBoxMsg << L"(" << GET_STRING( STR_ID_28125 ) << " ";
#else
					wstrmTalkBoxMsg << L"(to ";
#endif UPDATE_TO_STR_IN_WISPER
					wstrmTalkBoxMsg << wstrReceiverNickName << ") ";
					wstrmTalkBoxMsg << kEvent.m_wstrMsg;

					//wstrColor = L"#C2262FF";								// 22, 98, 255
					//coTextColor = D3DXCOLOR(0.08627f, 0.38431f, 1, 1);		// 22, 98, 255

					//wstrColor = L"#C009AFF";								// 0 154 255
					//coTextColor = D3DXCOLOR(0, 0.6f, 1, 1);					// 0 154 255

					wstrColor = L"#CFF3DE5";								// 255 61 229
					coTextColor = D3DXCOLOR(1, 0.239f, 0.898f, 1);			// 255 61 229


					if( bEmotionCommand == false )
					{
#ifdef CHAT_LOG_TO_TXT
						AddChatLogToFile( wstrmChatContent.str().c_str() );
#endif // CHAT_LOG_TO_TXT
						AddChatLog( wstrmChatContent.str().c_str(), KEGS_CHAT_REQ::CPT_WHISPER, coTextColor, wstrColor, false, bIsMyself );
					}


					if( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_SenderUnitUID )
					{
						m_wstrLastWhiperNickName = wstrReceiverNickName;
					}
					else
					{
						m_wstrLastWhiperNickName = wstrSenderNickName;
					}
				} break;
			}

			PopTalkBox( kEvent, wstrmTalkBoxMsg.str().c_str(), coTextColor );			

		} break;

	case KEGS_CHAT_REQ::CPT_PARTY:
		{
			CX2PartyManager::PartyMemberData* pPartyUserInfo_Sender = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( kEvent.m_SenderUnitUID );
			if( NULL == pPartyUserInfo_Sender )
				return false;

			wstringstream wstrmTalkBoxMsg;


			// 파티 채팅
			//wstring wstrColor = L"#CFF44E6";					// 255, 68, 230
			//D3DXCOLOR coTextColor(1, 0.266667f, 0.902f, 1);		// 255, 68, 230
			wstring wstrColor = L"#C42FFFF";					// 66, 255, 255
			D3DXCOLOR coTextColor(0.258f, 1, 1, 1);		// 66, 255, 255

			switch( kEvent.m_cRoomChatType )
			{
			case CX2Room::RCT_ALL:
			case CX2Room::RCT_TEAM:
				{
					wstring chatContent = pPartyUserInfo_Sender->m_wstrNickName;
					chatContent += L" : ";
					chatContent += kEvent.m_wstrMsg;

					wstrmTalkBoxMsg << kEvent.m_wstrMsg;
					if(bEmotionCommand == false)
					{
#ifdef CHAT_LOG_TO_TXT
						AddChatLogToFile( chatContent.c_str() );
#endif // CHAT_LOG_TO_TXT
						AddChatLog( chatContent.c_str(), KEGS_CHAT_REQ::CPT_PARTY, coTextColor, wstrColor, false, bIsMyself  );
					}
				} break;

			case CX2Room::RCT_WHISPER:
				{					
					CX2PartyManager::PartyMemberData* pPartyUserInfo_Receiver = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( kEvent.m_RecieverUnitUID );
					if( NULL != pPartyUserInfo_Receiver )
					{
						wstringstream wstrmChatContent;

						wstrmChatContent << pPartyUserInfo_Sender->m_wstrNickName << L" : (to ";
						wstrmChatContent << pPartyUserInfo_Receiver->m_wstrNickName << ") ";
						wstrmChatContent << kEvent.m_wstrMsg;

						wstrmTalkBoxMsg << L"(to ";
						wstrmTalkBoxMsg << pPartyUserInfo_Receiver->m_wstrNickName << ") ";
						wstrmTalkBoxMsg << kEvent.m_wstrMsg;

						//wstrColor = L"#C2262FF";								// 22, 98, 255
						//coTextColor = D3DXCOLOR(0.08627f, 0.38431f, 1, 1);		// 22, 98, 255
						wstrColor = L"#CFF3DE5";								// 255 61 229
						coTextColor = D3DXCOLOR(1, 0.239f, 0.898f, 1);			// 255 61 229

						if(bEmotionCommand == false)
						{
#ifdef CHAT_LOG_TO_TXT
							AddChatLogToFile( wstrmChatContent.str().c_str() );
#endif // CHAT_LOG_TO_TXT
							AddChatLog( wstrmChatContent.str().c_str(), KEGS_CHAT_REQ::CPT_WHISPER, coTextColor, wstrColor, false, bIsMyself  );
						}

						if( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_SenderUnitUID )
						{
							m_wstrLastWhiperNickName = pPartyUserInfo_Receiver->m_wstrNickName;
						}
						else
						{
							m_wstrLastWhiperNickName = pPartyUserInfo_Sender->m_wstrNickName;
						}
					}
				} break;
			}


			//{{ 허상형 : [2009/11/2] //	말풍선 색깔을 바꾼다
			D3DXCOLOR coBackColor(0.427f, 0.812f, 0.965f, 1);			// 109, 207, 246
			coTextColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1);

			PopTalkBox( kEvent, wstrmTalkBoxMsg.str().c_str(), coTextColor, D3DXCOLOR(0, 0, 0, 0), coBackColor );	

		} break;

	// 길드 채팅
	case KEGS_CHAT_REQ::CPT_GUILD:
		{
			wstring wstrSenderNickName = L"";
			wstring wstrReceiverNickName = L"";
			wstringstream wstrmTalkBoxMsg;

			wstrSenderNickName = kEvent.m_wstrSenderUnitNickName;

			wstring wstrColor = L"#C96ff00";									// 150, 255, 0
			D3DXCOLOR coTextColor = D3DXCOLOR(0.5882f, 1, 0, 1);				// 150, 255, 0
			D3DXCOLOR outLineColor(0, 0, 0, 1);

			switch( kEvent.m_cRoomChatType )
			{
			case CX2Room::RCT_ALL:
			case CX2Room::RCT_TEAM:
				{
					wstring chatContent = wstrSenderNickName;
					chatContent += L" : ";
					chatContent += kEvent.m_wstrMsg;
					wstrmTalkBoxMsg << kEvent.m_wstrMsg;

					if(bEmotionCommand == false)
					{
#ifdef CHAT_LOG_TO_TXT
						AddChatLogToFile( chatContent.c_str() );
#endif // CHAT_LOG_TO_TXT
						AddChatLog( chatContent.c_str(), KEGS_CHAT_REQ::CPT_GUILD, coTextColor, wstrColor, false, bIsMyself );										
					}
				} break;

			default:	/// kimhc // 길드에서는 귓속말 처리 안해도 되나??
				break;
			}

			//{{ 허상형 : [2009/11/2] //	말풍선 색깔을 바꾼다(글씨색은 검은색으로)
			D3DXCOLOR coBackColor(0.725f, 0.996f, 0.302f, 1);	// 185, 254, 77
			coTextColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1);
			PopTalkBox( kEvent, wstrmTalkBoxMsg.str().c_str(), coTextColor, D3DXCOLOR(0, 0, 0, 0), coBackColor );	

		} break;

	case KEGS_CHAT_REQ::CPT_WHISPER:
		{
			wstringstream wstrmChatContent;

			wstrmChatContent << kEvent.m_wstrSenderUnitNickName << L" : (to ";
			wstrmChatContent << kEvent.m_wstrRecieverUnitNickName << L") "; 
			wstrmChatContent << kEvent.m_wstrMsg;

			if( 0 == kEvent.m_wstrSenderUnitNickName.compare( g_pData->GetMyUser()->GetSelectUnit()->GetNickName() ) )
			{
				m_wstrLastWhiperNickName = kEvent.m_wstrRecieverUnitNickName;
			}
			else
			{
				m_wstrLastWhiperNickName = kEvent.m_wstrSenderUnitNickName;
			}

			wstring wstrColor = L"#CFF3DE5";								// 255 61 229
			D3DXCOLOR coTextColor = D3DXCOLOR(1, 0.239f, 0.898f, 1);			// 255 61 229

			if(bEmotionCommand == false)
				AddChatLog( wstrmChatContent.str().c_str(), KEGS_CHAT_REQ::CPT_WHISPER, coTextColor, wstrColor, false, bIsMyself );

		} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case KEGS_CHAT_REQ::CPT_RELATIONSHIP:
		{
			wstring wstrSenderNickName = L"";
			wstring wstrReceiverNickName = L"";
			wstringstream wstrmTalkBoxMsg;

			wstrSenderNickName = kEvent.m_wstrSenderUnitNickName;

			wstring wstrColor = L"#CFF7E00";								// 255 126 0
			D3DXCOLOR coTextColor = D3DXCOLOR(1, 0.494f, 0.0f, 1);			// 255 126 0
			D3DXCOLOR outLineColor(0, 0, 0, 1);

			switch( kEvent.m_cRoomChatType )
			{
			case CX2Room::RCT_ALL:
			case CX2Room::RCT_TEAM:
				{
					wstring chatContent = wstrSenderNickName;
					chatContent += L" : ";
					chatContent += kEvent.m_wstrMsg;
					wstrmTalkBoxMsg << kEvent.m_wstrMsg;

					if(bEmotionCommand == false)
					{
#ifdef CHAT_LOG_TO_TXT
						AddChatLogToFile( chatContent.c_str() );
#endif // CHAT_LOG_TO_TXT
						AddChatLog( chatContent.c_str(), KEGS_CHAT_REQ::CPT_RELATIONSHIP, coTextColor, wstrColor, false, bIsMyself );
					}
				} break;

			default:	/// kimhc // 길드에서는 귓속말 처리 안해도 되나??
				break;
			}

			//{{ 허상형 : [2009/11/2] //	말풍선 색깔을 바꾼다(글씨색은 검은색으로)
			D3DXCOLOR coBackColor( 0.996f, 0.725f, 0.302f, 1);
			coTextColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1);
			PopTalkBox( kEvent, wstrmTalkBoxMsg.str().c_str(), coTextColor, D3DXCOLOR(0, 0, 0, 0), coBackColor );	

		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	case KEGS_CHAT_REQ::CPT_SYSTEM:
		{

		} break;

	}

	DialogLog( L"ChatNotEnd" )

		return true;
}


bool CX2ChatBox::Handler_EGS_NEW_BLACKLIST_USER_REQ( const wstring& wstrNickName )
{
	KEGS_NEW_BLACKLIST_USER_REQ kPacket;
	kPacket.m_wstrNickName = wstrNickName;

	g_pData->GetServerProtocol()->SendPacket( EGS_NEW_BLACKLIST_USER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_NEW_BLACKLIST_USER_ACK, 60.0f );

	return true;
}

bool CX2ChatBox::Handler_EGS_NEW_BLACKLIST_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NEW_BLACKLIST_USER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_NEW_BLACKLIST_USER_ACK ) == true )
	{		
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AddBlackList( kEvent.m_kChatBlackListUnit );
			}
			//if( true == g_pMain->GetGameOption().AddBlackList( kEvent.m_iUnitUID, kEvent.m_wstrBlackListNick ) )
			{
				//AddBlackListWindow( kEvent.m_wstrBlackListNick );
			}

			wstring wstrColor = L"#CFF3F4D";							// 255, 63, 77
			D3DXCOLOR coTextColor(1.f, 0.24705f, 0.30196f, 1.f);		// 255, 63, 77

			AddChatLog( GET_REPLACED_STRING( ( STR_ID_59, "L", kEvent.m_kChatBlackListUnit.m_wstrNickName ) ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );

			return true;
		}
	}
	return false;
}

bool CX2ChatBox::Handler_EGS_DEL_BLACKLIST_USER_REQ( UidType unitUID )
{
	KEGS_DEL_BLACKLIST_USER_REQ kPacket;
	kPacket.m_iUnitUID = unitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_DEL_BLACKLIST_USER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_DEL_BLACKLIST_USER_ACK, 60.0f );

	return true;
}

bool CX2ChatBox::Handler_EGS_DEL_BLACKLIST_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DEL_BLACKLIST_USER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_DEL_BLACKLIST_USER_ACK ) == true )
	{		
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				g_pData->GetMyUser()->GetSelectUnit()->RemoveBlackList( kEvent.m_kChatBlackListUnit.m_iUnitUID );
			}
			//if( true == g_pMain->GetGameOption().RemoveBlackList( kEvent.m_iUnitUID, kEvent.m_wstrBlackListNick ) )
			{
				//RemoveBlackListWindow( kEvent.m_wstrBlackListNick );
			}


			wstring wstrColor = L"#CFF3F4D";							// 255, 63, 77
			D3DXCOLOR coTextColor(1.f, 0.24705f, 0.30196f, 1.f);		// 255, 63, 77
			AddChatLog( GET_REPLACED_STRING( ( STR_ID_60, "L", kEvent.m_kChatBlackListUnit.m_wstrNickName ) ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );


			return true;
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
void CX2ChatBox::SetStage( CKTDXStage* pStage )
{
	m_pStage = pStage;


	if( NULL != m_pDLGChatWindow )
	{
		m_pDLGChatWindow->SetStage( pStage );
	}

	if( NULL != m_pDLGChatWindowButton )
	{
		m_pDLGChatWindowButton->SetStage( pStage );
	}

	if( NULL != m_pDLGChatWindowEditBox )
	{
		m_pDLGChatWindowEditBox->SetStage( pStage );
	}
}


void CX2ChatBox::OpenChatWindowButton()
{
	if( NULL != m_pDLGChatWindowButton )
	{
		m_pDLGChatWindowButton->SetShowEnable( true, true );		
	}

	SetChatBoxLayerUp(true);
}

void CX2ChatBox::CloseChatWindowButton()
{
	if( NULL != m_pDLGChatWindowButton )
	{
		m_pDLGChatWindowButton->SetShowEnable( false, false );		
	}

	SetChatBoxLayerUp(false);
	m_bOpenChatWindowButton = false;

}


void CX2ChatBox::HideChatEditBox()
{

	if( NULL == m_pDLGChatWindowEditBox )
		return;

	m_pDLGChatWindowEditBox->ClearFocus();
	m_pDLGChatWindowEditBox->SetShowEnable( false, false );

	if(g_pX2Game != NULL)
		g_pX2Game->SetOpenChatBox( false );
}


void CX2ChatBox::UnHideChatEditBox()
{
	if( NULL == m_pDLGChatWindowEditBox )
		return;

	SetScrollBarEndPos();

	m_pDLGChatWindowEditBox->SetShowEnable( true, true );

	CKTDGUIIMEEditBox* pEditChat = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );
	if( NULL != pEditChat )
	{
		if( GetIMEEditWindowMode() == CWM_PERSONAL )
			ChangeIMEEditBox( CWM_PERSONAL );
		
		pEditChat->SetShowEnable( true, true );
		pEditChat->RequestFocus();
	}
}


void CX2ChatBox::HideChatWindowFrame()
{
	if( NULL == m_pDLGChatWindow )
		return;

	m_bHideChatWindowFrame = true;

//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
	// 채팅 출력창의 포커스를 클리어하려는데 채팅입력차의 포커스가 클리어되는 경우 막음
	if ( GetFocusChatEditBox() == false )
#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.12 //	채팅창 개편
	m_pDLGChatWindow->ClearFocus();
	//채팅창 구분변경 확장/비확장 -> 일반/메가폰
	CKTDGUIStatic* pStatic_BG_Common = (CKTDGUIStatic*) m_pDLGChatWindow->GetControl( L"Chat_Box_Common" );
	CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGChatWindow->GetControl( L"Chat_Box_Megaphone" );

	if( NULL != pStatic_BG )
	{
		pStatic_BG->SetShowEnable( false, false );
	}
	if( NULL != pStatic_BG_Common )
	{
		pStatic_BG_Common->SetShowEnable( false, false );
	}
	if( NULL != m_pDLGTabMenu )
	{
		m_pDLGTabMenu->SetShowEnable(false,false);
	}

	//CKTDGUIStatic* pStatic_BG_Base1 = (CKTDGUIStatic*) m_pDLGChatWindow->GetControl( L"Other_Box1" );
	//CKTDGUIStatic* pStatic_BG_Base2 = (CKTDGUIStatic*) m_pDLGChatWindow->GetControl( L"Other_Box2" );

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_TRAINING_GAME:
		{
			//pStatic_BG_Base1->SetShow(false);
			//pStatic_BG_Base2->SetShow(false);
		}
		break;
	default:
		{
#if 0 
			if(m_bChatWindowEnlarged == true)
			{
				pStatic_BG_Base1->SetShow(true);
				pStatic_BG_Base2->SetShow(true);
			}
			else
			{
				pStatic_BG_Base1->SetShow(true);
				pStatic_BG_Base2->SetShow(false);
			}
#endif
		}
		break;
	}	


	CKTDGUIRadioButton* pRadio_All = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonAll" );
	CKTDGUIRadioButton* pRadio_Party = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonParty" );
	CKTDGUIRadioButton* pRadio_Guild = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonGuild" );
	CKTDGUIRadioButton* pRadio_Personal = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonPersonal" );
	
	if( pRadio_All != NULL ) pRadio_All->SetShowEnable( false, false );
	if( pRadio_Party != NULL ) pRadio_Party->SetShowEnable( false, false );
	if( pRadio_Guild != NULL ) pRadio_Guild->SetShowEnable( false, false );
	if( pRadio_Personal != NULL ) pRadio_Personal->SetShowEnable( false, false );

	CKTDGUIButton* pButton_ScrollEnd = (CKTDGUIButton*) m_pDLGChatWindow->GetControl( L"Down2" );
	if( NULL != pButton_ScrollEnd )
	{
		pButton_ScrollEnd->SetShowEnable( false, false );
	}

	CKTDGUIButton* pButton_Help = (CKTDGUIButton*) m_pDLGChatWindow->GetControl( L"Help" );
	if( NULL != pButton_Help )
	{
		pButton_Help->SetShowEnable( false, false );
	}
	pButton_ScrollEnd = (CKTDGUIButton*) m_pDLGChatWindow->GetControl( L"MegaDown2" );
	if( NULL != pButton_ScrollEnd )
	{
		pButton_ScrollEnd->SetShowEnable( false, false );
	}

#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIRadioButton* pRadio_Relation_Couple = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonRelationCouple" );
	if( pRadio_Relation_Couple != NULL ) pRadio_Relation_Couple->SetShowEnable( false, false );

	CKTDGUIRadioButton* pRadio_Relation_Married = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonRelationMarried" );
	if( pRadio_Relation_Married != NULL ) pRadio_Relation_Married->SetShowEnable( false, false );
#endif // ADDED_RELATIONSHIP_SYSTEM



	CKTDGUIListBox* pListBox_All = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_All" );
	CKTDGUIListBox* pListBox_Party = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Party" );
	CKTDGUIListBox* pListBox_Guild = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Guild" );
	CKTDGUIListBox* pListBox_Personal = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Personal" );
	//{{ 허상형 : [2009/7/17] //	메가폰 탭 스크롤바 객체
	CKTDGUIListBox* pListBox_Megaphone = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Megaphone" );

#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIListBox* pListBox_Relation = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Relation" );
#endif // ADDED_RELATIONSHIP_SYSTEM


	//}} 허상형 : [2009/7/17] //	메가폰 탭 스크롤바 객체

	if( pListBox_All != NULL ) pListBox_All->GetScrollBar()->SetShowEnable( false, false );
	if( pListBox_Party != NULL ) pListBox_Party->GetScrollBar()->SetShowEnable( false, false );
	if( pListBox_Guild != NULL ) pListBox_Guild->GetScrollBar()->SetShowEnable( false, false );
	if( pListBox_Personal != NULL ) pListBox_Personal->GetScrollBar()->SetShowEnable( false, false );
	//{{ 허상형 : [2009/7/17] //	메가폰 탭 스크롤바 사라지기
	if( pListBox_Megaphone != NULL ) pListBox_Megaphone->GetScrollBar()->SetShowEnable( false, false );
	//}} 허상형 : [2009/7/17] //	메가폰 탭 스크롤바 사라지기
	
#ifdef ADDED_RELATIONSHIP_SYSTEM
	if( pListBox_Relation != NULL ) pListBox_Relation->GetScrollBar()->SetShowEnable( false, false );	
#endif // ADDED_RELATIONSHIP_SYSTEM

}

void CX2ChatBox::UnHideChatWindowFrame()
{
	if( NULL == m_pDLGChatWindow )
		return;

	m_bHideChatWindowFrame = false;
	//채팅창을 분리했기 떄문에 범위 변경
	CKTDGUIStatic* pStatic_BG_Common = (CKTDGUIStatic*) m_pDLGChatWindow->GetControl( L"Chat_Box_Common" );
	CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGChatWindow->GetControl( L"Chat_Box_Megaphone" );

	if( NULL != pStatic_BG )
	{
		pStatic_BG->SetShowEnable( true, true );
	}
	if( NULL != pStatic_BG_Common )
	{
		pStatic_BG_Common->SetShowEnable( true, true );
	}
	CKTDGUIRadioButton* pRadio_All = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonAll" );
	CKTDGUIRadioButton* pRadio_Party = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonParty" );
	CKTDGUIRadioButton* pRadio_Guild = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonGuild" );
	CKTDGUIRadioButton* pRadio_Personal = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonPersonal" );
	// 전체
	pRadio_All->SetShowEnable( true, true );

	// 파티
	pRadio_Party->SetShowEnable( true, true );
	
	//{{ 허상형 : [2009/9/16] //	길드 채팅 탭 활성화 설정
#ifdef GUILD_MANAGEMENT
	pRadio_Guild->SetShowEnable( true, true );
#else
	pRadio_Guild->SetShowEnable( true, false );
#endif
	//}} 허상형 : [2009/9/16] //	길드 채팅 탭 활성화 설정

	// 귓속말
	pRadio_Personal->SetShowEnable( true, true );

#ifdef ADDED_RELATIONSHIP_SYSTEM

	if ( NULL != g_pData->GetRelationshipManager() && 
		NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() ) 
	{
		switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
		{
			case SEnum::RT_SOLO :
				{
					CKTDGUIRadioButton* pRadio_Relation_Couple = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonRelationCouple" );
					if( pRadio_Relation_Couple != NULL ) pRadio_Relation_Couple->SetShowEnable( false, false );

					CKTDGUIRadioButton* pRadio_Relation_Married = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonRelationMarried" );
					if( pRadio_Relation_Married != NULL ) pRadio_Relation_Married->SetShowEnable( false, false );
				}
				break;

			case SEnum::RT_COUPLE :
				{
					CKTDGUIRadioButton* pRadio_Relation_Couple = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonRelationCouple" );
					if( pRadio_Relation_Couple != NULL ) pRadio_Relation_Couple->SetShowEnable( true, true );

					CKTDGUIRadioButton* pRadio_Relation_Married = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonRelationMarried" );
					if( pRadio_Relation_Married != NULL ) pRadio_Relation_Married->SetShowEnable( false, false );

				}
				break;

			case SEnum::RT_MARRIED :				
				{
					CKTDGUIRadioButton* pRadio_Relation_Couple = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonRelationCouple" );
					if( pRadio_Relation_Couple != NULL ) pRadio_Relation_Couple->SetShowEnable( false, false );

					CKTDGUIRadioButton* pRadio_Relation_Married = (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonRelationMarried" );
					if( pRadio_Relation_Married != NULL ) pRadio_Relation_Married->SetShowEnable( true, true );
				}
				break;
		}
	}
#endif // ADDED_RELATIONSHIP_SYSTEM


	CKTDGUIButton* pButton_ScrollEnd = (CKTDGUIButton*) m_pDLGChatWindow->GetControl( L"Down2" );
	if( NULL != pButton_ScrollEnd )
	{
		pButton_ScrollEnd->SetShowEnable( true, true );
	}
	CKTDGUIButton* pButton_Help = (CKTDGUIButton*) m_pDLGChatWindow->GetControl( L"Help" );
	if( NULL != pButton_Help )
	{
		pButton_Help->SetShowEnable( true, true );
	}
	pButton_ScrollEnd = (CKTDGUIButton*) m_pDLGChatWindow->GetControl( L"MegaDown2" );
	if( NULL != pButton_ScrollEnd )
	{
		pButton_ScrollEnd->SetShowEnable( true, true );
	}
	CKTDGUIListBox* pListBox_All = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_All" );
	CKTDGUIListBox* pListBox_Party = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Party" );
	CKTDGUIListBox* pListBox_Guild = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Guild" );
	CKTDGUIListBox* pListBox_Personal = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Personal" );
	//{{ 허상형 : [2009/7/17] //	메가폰 탭 스크롤바 객체
	CKTDGUIListBox* pListBox_Megaphone = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Megaphone" );
	//}} 허상형 : [2009/7/17] //	메가폰 탭 스크롤바 객체
#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIListBox* pListBox_Relation = static_cast <CKTDGUIListBox*> ( m_pDLGChatWindow->GetControl( L"ListBox_Relation" ) );
#endif // ADDED_RELATIONSHIP_SYSTEM


	// 전체
	pListBox_All->GetScrollBar()->SetShowEnable( true, true );
	// 파티	
	pListBox_Party->GetScrollBar()->SetShowEnable( true, true );	
	// 길드
	pListBox_Guild->GetScrollBar()->SetShowEnable( true, true );
	// 귓속말
	pListBox_Personal->GetScrollBar()->SetShowEnable( true, true );
	//{{ 허상형 : [2009/7/17] //	메가폰 탭 스크롤바 활성화
	pListBox_Megaphone->GetScrollBar()->SetShowEnable( true, true );
	//}} 허상형 : [2009/7/17] //	메가폰 탭 스크롤바 활성화
#ifdef ADDED_RELATIONSHIP_SYSTEM
	pListBox_Relation->GetScrollBar()->SetShowEnable( true, true );
#endif // ADDED_RELATIONSHIP_SYSTEM

}

void CX2ChatBox::HideChatWindow()
{
	if( NULL != m_pDLGChatWindow )
	{
		m_pDLGChatWindow->ClearFocus();
		m_pDLGChatWindow->SetShowEnable( false, false );
	}

	if( NULL != m_pDLGChatWindowEditBox )
	{
		m_pDLGChatWindowEditBox->ClearFocus();
		m_pDLGChatWindowEditBox->SetShowEnable( false, false );
		if(g_pX2Game != NULL)
			g_pX2Game->SetOpenChatBox( false );
	}
}

void CX2ChatBox::UnHideChatWindow()
{
	if( NULL == m_pDLGChatWindow )
		return;

	if( NULL == m_pDLGChatWindowEditBox )
		return;

	if( true == m_bOpenChatWindow )
	{
		m_pDLGChatWindow->SetShowEnable( true, true );
		m_pDLGChatWindowEditBox->SetShowEnable( true, true );

		CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );
		if( NULL != pIMEEditBox )
			pIMEEditBox->RequestFocus();

//{{ kimhc // 2010.3.23 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
		// 09. 05. 13김정협 언어가 국어일경우만 아래함수실행하도록 버그수정
		if( m_bIsFirstOpenEditWindow == true && pIMEEditBox->IsKorean() )
		{
			m_bIsFirstOpenEditWindow = false;
#ifdef CHAT_KOREAN_REMOVE
			g_pMain->ChangeKoreanAndEnglishKey( false );
#else CHAT_KOREAN_REMOVE
			g_pMain->ChangeKoreanAndEnglishKey( true );
#endif CHAT_KOREAN_REMOVE
		}
#else	CHAT_WINDOW_IMPROV
		// 09. 05. 13김정협 언어가 국어일경우만 아래함수실행하도록 버그수정
		if(pIMEEditBox->IsKorean() )
			g_pMain->ChangeKoreanAndEnglishKey( true );
#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.23 //	채팅창 개편
	}
}

void CX2ChatBox::CloseChatWindow()
{
	//{{ 2007. 11. 13  최육사  안쓰는 통계정리	
	//if( m_bOpenUserListWindow == true ) // 창이 열려있다가 닫히는거면
	//{
	//	g_pData->GetServerProtocol()->SendID( EGS_CLOSE_WND_PARTY_NOT );
	//}
	//}}

	m_bOpenChatWindow = false;

	HideChatWindow();
}

void CX2ChatBox::OpenChatWindow()
{
	if( NULL == m_pDLGChatWindow )
		return;

	//{{ 허상형 : [2009/7/25] //	채팅 메시지를 입력하기위해 엔터를 쳤을때 사라졌던 기존 메시지가 보이지않게 수정
	//ChangeChatWindowAndIMEEditBox( m_eChatWindowMode );
	//}} 허상형 : [2009/7/25] //	채팅 메시지를 입력하기위해 엔터를 쳤을때 사라졌던 기존 메시지가 보이지않게 수정

	m_bOpenChatWindow = true;
	UnHideChatWindow();


	if( NULL != m_pDLGChatWindowButton )
	{
		m_pDLGChatWindowButton->MoveStop();
		m_pDLGChatWindowButton->SetColor( D3DXCOLOR(1,1,1,1) );
	}
}

wstring CX2ChatBox::GetChatMessage( bool bClear /*= true */)
{
	wstring chatMsg = L"";
	CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );

	m_pDLGChatWindowEditBox->ClearFocus();

	if( NULL != pIMEEditBox )
	{
		chatMsg = pIMEEditBox->GetText();
		DialogLog( L"GetChatMessageStart" )
			pIMEEditBox->ClearText();
		DialogLog( L"GetChatMessageEnd" )
	}

	// Limit
	if( chatMsg.length() > 100 )
	{
		chatMsg = chatMsg.substr(0, 100);
	}

	return chatMsg;
}

void CX2ChatBox::AddChatLogObtain( const WCHAR* wszMsg, CHAT_OBTAIN_TYPE eObtainType )
{
	if( NULL == m_pDLGChatWindow )
		return;

	D3DXVECTOR2 vTemp = g_pKTDXApp->ConvertByResolution( 330, 330 );
	const int CHAT_LINE_WIDTH = (int)vTemp.x;

	// CKTDGUIListBox::HandleMouse에 있는 MAGIC_SHOW_TIME도 동일하게 수정해야
	const float MAGIC_CHAT_SHOW_TIME = 50.0f;
	const int MAX_LINE = 100;

	int addRow = 0;
	wstring chatContent = wszMsg;

	wstring wstrColor = L"#C4EB2D2";								// 78, 178, 210
	D3DXCOLOR coTextColor(0.30588f, 0.69803f, 0.823529f, 1.f);		// 78, 178, 210
#ifdef CLIENT_GLOBAL_LINEBREAK
	CKTDGUIListBox* pListBoxChatContent = (CKTDGUIListBox*)m_pDLGChatWindow->GetControl( L"ListBox_All" );
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pListBoxChatContent->GetString()->fontIndex );
	addRow = CWordLineHandler::LineBreakInX2Main(chatContent, pFont, CHAT_LINE_WIDTH, wstrColor, true, false);
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
	addRow = LineBreak( chatContent, CHAT_LINE_WIDTH, wstrColor, true );
#endif //CLIENT_GLOBAL_LINEBREAK


	switch (eObtainType)
	{
	case COT_ED:
		{
			if( true == GetDisplayObtainingED(CWM_TOTAL) )
			{
				ListBoxAddItem(L"ListBox_All", chatContent, addRow);	
			}	
			if( true == GetDisplayObtainingED(CWM_PARTY) )
			{
				ListBoxAddItem(L"ListBox_Party", chatContent, addRow);	
			}	
			if( true == GetDisplayObtainingED(CWM_GUILD) )
			{
				ListBoxAddItem(L"ListBox_Guild", chatContent, addRow);	
			}	
			if( true == GetDisplayObtainingED(CWM_PERSONAL) )
			{
				ListBoxAddItem(L"ListBox_Personal", chatContent, addRow);	
			}

#ifdef ADDED_RELATIONSHIP_SYSTEM
			if ( NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
				{

				case SEnum::RT_COUPLE :
					if( true == GetDisplayObtainingED(CWM_RELATION_COUPLE) )
					{
						ListBoxAddItem(L"ListBox_Relation", chatContent, addRow);	
					}
					break;
				case SEnum::RT_MARRIED :
					if( true == GetDisplayObtainingED(CWM_RELATION_MARRIED) )
					{
						ListBoxAddItem(L"ListBox_Relation", chatContent, addRow);	
					}
					break;
				default :
					break;
				}
			}
#endif // ADDED_RELATIONSHIP_SYSTEM
		}
		break;
	case COT_EXP:
		{
			if( true == GetDisplayObtainingEXP(CWM_TOTAL) )
			{
				ListBoxAddItem(L"ListBox_All", chatContent, addRow);	
			}	
			if( true == GetDisplayObtainingEXP(CWM_PARTY) )
			{
				ListBoxAddItem(L"ListBox_Party", chatContent, addRow);	
			}	
			if( true == GetDisplayObtainingEXP(CWM_GUILD) )
			{
				ListBoxAddItem(L"ListBox_Guild", chatContent, addRow);	
			}	
			if( true == GetDisplayObtainingEXP(CWM_PERSONAL) )
			{
				ListBoxAddItem(L"ListBox_Personal", chatContent, addRow);	
			}
#ifdef ADDED_RELATIONSHIP_SYSTEM
			if ( NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
				{

				case SEnum::RT_COUPLE :
					if( true == GetDisplayObtainingEXP(CWM_RELATION_COUPLE) )
					{
						ListBoxAddItem(L"ListBox_Relation", chatContent, addRow);	
					}
					break;
				case SEnum::RT_MARRIED :
					if( true == GetDisplayObtainingEXP(CWM_RELATION_MARRIED) )
					{
						ListBoxAddItem(L"ListBox_Relation", chatContent, addRow);	
					}
					break;
				default :
					break;
				}
			}
#endif // ADDED_RELATIONSHIP_SYSTEM
		}
		break;
	}
}
void CX2ChatBox::AddChatLog( const WCHAR* wszMsg, KEGS_CHAT_REQ::CHAT_PACKET_TYPE eChatPacketType, 
								 D3DXCOLOR coTextColor /*= D3DXCOLOR(1,1,1,1)*/, wstring wstrColor /*= L""*/, bool bEnableColor /*= true*/,
								 bool bIsMyself /*= false*/, float fDelayTime /*= 0.f */)
{
	KTDXPROFILE();

	if( fDelayTime > 0.f )
	{
		QueuedChatLog queuedChagLog;
		queuedChagLog.m_wstrMsg = wszMsg;
		queuedChagLog.m_eChatPacketType = eChatPacketType;
		queuedChagLog.m_coTextColor = coTextColor;
		queuedChagLog.m_wstrColor = wstrColor;
		queuedChagLog.m_bEnableColor = bEnableColor;
		queuedChagLog.m_bIsMyself = bIsMyself;
		queuedChagLog.m_fRemainTime = fDelayTime;

		m_vecQueuedChatLog.push_back( queuedChagLog );
		return;
	}


	if( NULL == m_pDLGChatWindow )
		return;
	D3DXVECTOR2 vTemp = g_pKTDXApp->ConvertByResolution( 330, 330 );
	const int CHAT_LINE_WIDTH = (int)vTemp.x;


	// CKTDGUIListBox::HandleMouse에 있는 MAGIC_SHOW_TIME도 동일하게 수정해야
	const float MAGIC_CHAT_SHOW_TIME = 50.0f;
	const int MAX_LINE = 100;

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_TRAINING_GAME:
		{

		} break;

	default:
		{
			if( false == GetOpenChatWindow() && NULL != m_pDLGChatWindowButton )
			{				
				if( eChatPacketType == KEGS_CHAT_REQ::CPT_WHISPER || eChatPacketType == KEGS_CHAT_REQ::CPT_PARTY
					|| eChatPacketType == KEGS_CHAT_REQ::CPT_MEGAPHONE 

					)
				{
					if( false == m_pDLGChatWindowButton->GetIsMoving() )
					{
						m_pDLGChatWindowButton->Move( m_pDLGChatWindowButton->GetPos(), D3DXCOLOR(1,1,1,1), 0.5f, true, false, false, 1 );
						m_pDLGChatWindowButton->Move( m_pDLGChatWindowButton->GetPos(), D3DXCOLOR(0,0,0,1), 0.5f, false, false, true, 50 );
					}
				}				
			}
		} break;
	}

	//{{ 허상형 : [2009/7/21] //	메가폰 메시지 처리 부분
	if( m_wstrSenderName != L"" )
	{
		CKTDGUIListBox* pListBoxMegaphone = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Megaphone" );
		if( NULL != pListBoxMegaphone )
		{
#ifdef CLIENT_GLOBAL_LINEBREAK
			m_iMegaID = (m_iMegaID + 1) % 100000000;
			CKTDGUIListBox* pListBoxChatContent = (CKTDGUIListBox*)m_pDLGChatWindow->GetControl( L"ListBox_All" );
			CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pListBoxChatContent->GetString()->fontIndex );

			CWordLineHandler::MegaLineBreakAdd( wszMsg, pFont, CHAT_LINE_WIDTH, pListBoxMegaphone, m_iMegaID, m_wstrSenderName );	
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
			MegaLineBreakAdd( wszMsg, CHAT_LINE_WIDTH, pListBoxMegaphone );
#endif //CLIENT_GLOBAL_LINEBREAK
		}
		return;
	}
	//}} 허상형 : [2009/7/21] //	메가폰 메시지 처리 부분


#if 0 // 자신의 글자색을 특정색으로 구분시킨다.
	if( true == bIsMyself )
	{
		wstrColor = L"#CFFF579";										// 255, 245, 121
		coTextColor = D3DXCOLOR(1, 0.266667f, 0.96f, 0.4745f);			// 255, 245, 121
	}
	else
	{
		// 파티 채팅이면 색상을 다르게~
		if( eChatPacketType == KEGS_CHAT_REQ::CPT_PARTY && wstrColor.length() == 0 )
		{
			//wstrColor = L"#CFF44E6";									// 255, 68, 230
			//coTextColor = D3DXCOLOR(1, 0.266667f, 0.902f, 1);			// 255, 68, 230
			wstring wstrColor = L"#C42FFFF";					// 66, 255, 255
			D3DXCOLOR coTextColor(0.258f, 1, 1, 1);		// 66, 255, 255
		}

	}
#else	// 자신의 글자색은 따로 구분하지 않는다.
	// 파티 채팅이면 색상을 다르게~
	if( eChatPacketType == KEGS_CHAT_REQ::CPT_PARTY && wstrColor.length() == 0 )
	{
		//wstrColor = L"#CFF44E6";									// 255, 68, 230
		//coTextColor = D3DXCOLOR(1, 0.266667f, 0.902f, 1);			// 255, 68, 230
		wstring wstrColor = L"#C42FFFF";					// 66, 255, 255
		D3DXCOLOR coTextColor(0.258f, 1, 1, 1);		// 66, 255, 255
	}
#endif	
	int addRow = 0;
	wstring chatContent = wszMsg;

//{{ 오현빈 // 2012-04-20 // 채팅 기록 여부를 각각의 탭 마다 다른 옵션을 가질 수 있도록 하기 위해 수정
#ifdef CLIENT_GLOBAL_LINEBREAK
	CKTDGUIListBox* pListBoxChatContent = (CKTDGUIListBox*)m_pDLGChatWindow->GetControl( L"ListBox_All" );
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pListBoxChatContent->GetString()->fontIndex );
			//2013/5/9 라인브레이크통합
			if( NULL == pFont )
				return;
				
	addRow = CWordLineHandler::LineBreakInX2Main( chatContent, pFont, CHAT_LINE_WIDTH, wstrColor, bEnableColor, true );
#else //CLIENT_GLOBAL_LINEBREAK
	addRow = LineBreak( chatContent, CHAT_LINE_WIDTH, wstrColor, bEnableColor );
#endif CLIENT_GLOBAL_LINEBREAK
	switch( eChatPacketType )
	{
	case KEGS_CHAT_REQ::CPT_TOTAL:
		{		
			if( true == GetDisplayNomal(CWM_TOTAL) )
			{
				ListBoxAddItem(L"ListBox_All", chatContent, addRow);	
			}
		}
		break;
	case KEGS_CHAT_REQ::CPT_PARTY:
		{			
			if( true == GetDisplayParty(CWM_PARTY) )
			{
				ListBoxAddItem(L"ListBox_Party", chatContent, addRow);
			}
			if( true == GetDisplayParty(CWM_TOTAL) )
			{
				ListBoxAddItem(L"ListBox_All", chatContent, addRow);
			}
		}
		break;
	case KEGS_CHAT_REQ::CPT_SYSTEM:
		{			
			if( true == GetDisplaySystem(CWM_TOTAL) )
			{
				ListBoxAddItem(L"ListBox_All", chatContent, addRow);	
			}	
			if( true == GetDisplaySystem(CWM_PARTY) )
			{
				ListBoxAddItem(L"ListBox_Party", chatContent, addRow);	
			}	
			if( true == GetDisplaySystem(CWM_GUILD) )
			{
				ListBoxAddItem(L"ListBox_Guild", chatContent, addRow);	
			}	
			if( true == GetDisplaySystem(CWM_PERSONAL) )
			{
				ListBoxAddItem(L"ListBox_Personal", chatContent, addRow);	
			}
#ifdef ADDED_RELATIONSHIP_SYSTEM
			if ( NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
				{

				case SEnum::RT_COUPLE :
					if( true == GetDisplaySystem(CWM_RELATION_COUPLE) )
					{
						ListBoxAddItem(L"ListBox_Relation", chatContent, addRow);	
					}
					break;
				case SEnum::RT_MARRIED :
					if( true == GetDisplaySystem(CWM_RELATION_MARRIED) )
					{
						ListBoxAddItem(L"ListBox_Relation", chatContent, addRow);	
					}
					break;
				default :
					break;
				}
			}
#endif //  ADDED_RELATIONSHIP_SYSTEM
		}
		break;
	case KEGS_CHAT_REQ::CPT_GUILD:
		{			
			if( true == GetDisplayGuild(CWM_GUILD) )
			{
				ListBoxAddItem(L"ListBox_Guild", chatContent, addRow);
			}
			if( true == GetDisplayGuild(CWM_TOTAL) )
			{
				ListBoxAddItem(L"ListBox_All", chatContent, addRow);
			}
		}
		break;
	case KEGS_CHAT_REQ::CPT_WHISPER:
		{				
			if( true == GetDisplayWhisper(CWM_TOTAL) )
			{
				ListBoxAddItem(L"ListBox_All", chatContent, addRow);	
			}	
			if( true == GetDisplayWhisper(CWM_PARTY) )
			{
				ListBoxAddItem(L"ListBox_Party", chatContent, addRow);	
			}	
			if( true == GetDisplayWhisper(CWM_GUILD) )
			{
				ListBoxAddItem(L"ListBox_Guild", chatContent, addRow);	
			}	
			if( true == GetDisplayWhisper(CWM_PERSONAL) )
			{
				ListBoxAddItem(L"ListBox_Personal", chatContent, addRow);	
			}
#ifdef ADDED_RELATIONSHIP_SYSTEM
			if ( NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
				{

				case SEnum::RT_COUPLE :
					if( true == GetDisplayWhisper(CWM_RELATION_COUPLE) )
					{
						ListBoxAddItem(L"ListBox_Relation", chatContent, addRow);	
					}
					break;
				case SEnum::RT_MARRIED :
					if( true == GetDisplayWhisper(CWM_RELATION_MARRIED) )
					{
						ListBoxAddItem(L"ListBox_Relation", chatContent, addRow);	
					}
					break;
				default :
					break;
				}
			}
#endif // ADDED_RELATIONSHIP_SYSTEM
		}
		break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case KEGS_CHAT_REQ::CPT_RELATIONSHIP :
		{
			if( true == GetDisplayRelation(CWM_TOTAL) )
			{
				ListBoxAddItem(L"ListBox_All", chatContent, addRow);	
			}	
/*			if( true == GetDisplayRelation(CWM_PARTY) )
			{
				ListBoxAddItem(L"ListBox_Party", chatContent, addRow);	
			}	
			if( true == GetDisplayRelation(CWM_GUILD) )
			{
				ListBoxAddItem(L"ListBox_Guild", chatContent, addRow);	
			}	
			if( true == GetDisplayWhisper(CWM_PERSONAL) )
			{
				ListBoxAddItem(L"ListBox_Personal", chatContent, addRow);	
			}
*/
			if( true == GetDisplayRelation(CWM_PARTY) )
			{
				ListBoxAddItem(L"ListBox_Party", chatContent, addRow);	
			}
			if ( NULL != g_pData->GetRelationshipManager() &&
				 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
				{
		
				case SEnum::RT_COUPLE :
					if( true == GetDisplayRelation(CWM_RELATION_COUPLE) )
					{
						ListBoxAddItem(L"ListBox_Relation", chatContent, addRow);	
					}
					break;
				case SEnum::RT_MARRIED :
					if( true == GetDisplayRelation(CWM_RELATION_MARRIED) )
					{
						ListBoxAddItem(L"ListBox_Relation", chatContent, addRow);	
					}
					break;
				default :
					break;
				}
			}
		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM


	case KEGS_CHAT_REQ::CPT_MEGAPHONE:
		{			
			ListBoxAddItem(L"ListBox_Megaphone", chatContent, addRow);
		}
		break;
	default:
		break;
	}
}

int CX2ChatBox::LineBreak( wstring& wstrText, int iTextWidth, const wstring& wstrTextColor, bool bEnableColor )
{
	KTDXPROFILE();

	enum CHAR_STATE
	{
		Q_NORMAL,
		Q_SHARP,
		Q_SHARP_C,
	};

    //{{ seojt // 2009-8-21, 11:57
    if( m_pDLGChatWindow == NULL )
    {
        return 0;
    }//if
    //}} seojt // 2009-8-21, 11:57

	CKTDGUIListBox* pListBoxChatContent = (CKTDGUIListBox*)m_pDLGChatWindow->GetControl( L"ListBox_All" );
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pListBoxChatContent->GetString()->fontIndex );


	int iNowCharWidth = 0;		
	int iNowLineWidth = 0;
	int iNowLineCount = 0;
	int iNowCharIndex = 0;
	wstring wstrLineBuf = L"";
	wstring wstrColorCode = L"";

	CHAR_STATE eNowCharState = Q_NORMAL;
	bool bIsFirstCharOfLine = true; 


	if( 0 != wstrTextColor.length() )
	{
		wstrLineBuf += wstrTextColor;
	}

	while( iNowCharIndex < (int) wstrText.length() )
	{
		WCHAR wszCharBuf = wstrText[iNowCharIndex];
		wstring wstrCharBuf = wstrText.substr( iNowCharIndex, 1 );

		switch( eNowCharState )
		{
		case Q_NORMAL:
			{
				if( 0 == wstrCharBuf.compare( L"#" ) )
				{
					if( true == bEnableColor )
					{
						wstrLineBuf += wstrCharBuf;
						eNowCharState = Q_SHARP;
					}
					else
					{
						wstrLineBuf += L"*"; 
						iNowCharWidth = pFont->GetWidth( L"*" );
						iNowLineWidth += iNowCharWidth;
					}
				}
				else if( 0 == wstrCharBuf.compare( L"\n" ) )
				{
					if( bIsFirstCharOfLine == false )
					{
						wstrLineBuf += wstrCharBuf;
						wstrLineBuf += L"  ";
						iNowLineCount++;
						iNowLineWidth = 0;
						bIsFirstCharOfLine = true;

						if( 0 != wstrTextColor.length() )
						{
							wstrLineBuf += wstrTextColor;
						}

						if( 0 != wstrColorCode.length() )
						{
							wstrLineBuf += wstrColorCode;
						}
					}
				}
				else
				{
					if( 0 != wstrCharBuf.compare( L" " ) || false == bIsFirstCharOfLine )
					{
						if( true == bIsFirstCharOfLine )
							bIsFirstCharOfLine = false;
						wstrLineBuf += wstrCharBuf;
						iNowCharWidth = pFont->GetWidth( wszCharBuf );
						iNowLineWidth += iNowCharWidth;
					}
				}

			} break;

		case Q_SHARP:
			{
				if( 0 == wstrCharBuf.compare( L"C" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP_C;
				}
				else
				{
					if( true == bIsFirstCharOfLine )
						bIsFirstCharOfLine = false;

					wstrLineBuf += wstrCharBuf;
					iNowCharWidth = pFont->GetWidth( wszCharBuf );
					iNowLineWidth += iNowCharWidth;

					eNowCharState = Q_NORMAL;
				}
			} break;

		case Q_SHARP_C:
			{
				if( 0 == wstrCharBuf.compare( L"X" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrColorCode = L"";

					if( 0 != wstrTextColor.length() )
					{
						wstrLineBuf += wstrTextColor;
					}

					eNowCharState = Q_NORMAL;
				}
				else // RGB 
				{
					// fix!! color code가 올바른지 체크하는 코드 있어야 됨. 일단 color code는 무조건 올바른것으로 가정
					wstrColorCode = L"#C";
					wstrColorCode += wstrText.substr( iNowCharIndex, 6 );
					wstrLineBuf += wstrText.substr( iNowCharIndex, 6 );
					iNowCharIndex += 5;
					eNowCharState = Q_NORMAL;
				}

			} break;
		}

		if( iNowLineWidth >= iTextWidth && Q_NORMAL == eNowCharState )
		{
			wstrLineBuf += L"\n";
			wstrLineBuf += L"  ";
			iNowLineCount++;
			iNowLineWidth = 0;
			bIsFirstCharOfLine = true;

			if( 0 != wstrTextColor.length() )
			{
				wstrLineBuf += wstrTextColor;
			}

			if( 0 != wstrColorCode.length() )
			{
				wstrLineBuf += wstrColorCode;
			}
		}

		iNowCharIndex++;
	}


	wstrText = wstrLineBuf;

	return iNowLineCount;
}

//{{ 허상형 : [2009/7/20] //	메가폰용 LineBreak, 나눈 값들을 바로 전송한다.
bool CX2ChatBox::MegaLineBreakAdd( wstring wstrText, int iTextWidth, CKTDGUIListBox* pListBox )
{
	enum CHAR_STATE
	{
		Q_NORMAL,
		Q_SHARP,
		Q_SHARP_C,
	};

	CKTDGUIListBox* pListBoxChatContent = (CKTDGUIListBox*)m_pDLGChatWindow->GetControl( L"ListBox_All" );
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pListBoxChatContent->GetString()->fontIndex );

	int iNowCharWidth = 0;		
	int iNowLineWidth = 0;
	int iNowLineCount = 0;
	int iNowCharIndex = 0;
	wstring wstrColorCode = L"";
	wstring wstrLineBuf = L"";

	const float MAGIC_CHAT_SHOW_TIME = 50.f;
	const int MAX_LINE = 100;

	//	메가폰 ID 설정
	m_iMegaID = (m_iMegaID + 1) % 100000000;

	CHAR_STATE eNowCharState = Q_NORMAL;
	bool bIsFirstCharOfLine = true; 
	bool bIsMyMessage = false;

	const int iLastTrackPos = pListBox->GetScrollBar()->GetTrackPos() + pListBox->GetScrollBar()->GetPageSize();
	const int iLastTrackEndPos = pListBox->GetScrollBar()->GetTrackEndPos();

	//	자기 메시지인지 구분
	if( m_wstrSenderName == g_pData->GetMyUser()->GetSelectUnit()->GetNickName() )
	{
		bIsMyMessage = true;
	}

	while( pListBox->GetSize() > MAX_LINE )
	{
		pListBox->RemoveItem(0);
	}

	while( iNowCharIndex < (int) wstrText.length() )
	{
		WCHAR wszCharBuf = wstrText[iNowCharIndex];
		wstring wstrCharBuf = wstrText.substr( iNowCharIndex, 1 );

		switch( eNowCharState )
		{
		case Q_NORMAL:
			{
				if( 0 == wstrCharBuf.compare( L"#" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP;
				}
				else if( 0 == wstrCharBuf.compare( L"\n" ) )
				{
					if( bIsFirstCharOfLine == false )
					{
						wstrLineBuf += wstrCharBuf;
						wstrLineBuf += L"  ";
						iNowLineCount++;
						iNowLineWidth = 0;
						bIsFirstCharOfLine = true;

						if( 0 != wstrColorCode.length() )
						{
							wstrLineBuf += wstrColorCode;
						}
					}
				}
				else
				{
					if( 0 != wstrCharBuf.compare( L" " ) || false == bIsFirstCharOfLine )
					{
						if( true == bIsFirstCharOfLine )
							bIsFirstCharOfLine = false;
						wstrLineBuf += wstrCharBuf;
						iNowCharWidth = pFont->GetWidth( wszCharBuf );
						iNowLineWidth += iNowCharWidth;
					}
				}

			} break;

		case Q_SHARP:
			{
				if( 0 == wstrCharBuf.compare( L"C" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP_C;
				}
				else
				{
					if( true == bIsFirstCharOfLine )
						bIsFirstCharOfLine = false;

					wstrLineBuf += wstrCharBuf;
					iNowCharWidth = pFont->GetWidth( wszCharBuf );
					iNowLineWidth += iNowCharWidth;

					eNowCharState = Q_NORMAL;
				}
			} break;

		case Q_SHARP_C:
			{
				if( 0 == wstrCharBuf.compare( L"X" ) )
				{
					wstrLineBuf += wstrCharBuf;

					eNowCharState = Q_NORMAL;
				}
				else // RGB 
				{
					// fix!! color code가 올바른지 체크하는 코드 있어야 됨. 일단 color code는 무조건 올바른것으로 가정
					wstrColorCode = L"#C";
					wstrColorCode += wstrText.substr( iNowCharIndex, 6 );
					wstrLineBuf += wstrText.substr( iNowCharIndex, 6 );
					iNowCharIndex += 5;
					eNowCharState = Q_NORMAL;
				}
			} break;
		}

		if( iNowLineWidth >= iTextWidth && Q_NORMAL == eNowCharState )
		{
			pListBox->AddItem( wstrLineBuf.c_str(), NULL, m_iMegaID, m_wstrSenderName, bIsMyMessage );

			wstrLineBuf = L"  #CFFAE00";
			iNowLineCount++;
			iNowLineWidth = 0;
			bIsFirstCharOfLine = true;

			CKTDGUIListBox::ListBoxItem* pItem = pListBox->GetLastItem();

			if( NULL != pItem )
			{
				pItem->m_bShow = true;
				pItem->m_fTimeLeftShow = MAGIC_CHAT_SHOW_TIME; 
			}
		}

		iNowCharIndex++;
	}

	if(iNowLineWidth != 0)
	{
		pListBox->AddItem( wstrLineBuf.c_str(), NULL, m_iMegaID, m_wstrSenderName, bIsMyMessage );
	}

	CKTDGUIListBox::ListBoxItem* pItem = pListBox->GetLastItem();
	if( NULL != pItem )
	{
		pItem->m_bShow = true;
		pItem->m_fTimeLeftShow = MAGIC_CHAT_SHOW_TIME; 
	}

	if( iLastTrackPos == iLastTrackEndPos ) 
	{
		pListBox->SetScrollBarEndPos();
	}

	return true;
}
//}} 허상형 : [2009/7/20] //	메가폰용 LineBreak, 나눈 값들을 바로 전송한다.


void CX2ChatBox::ChangeChatWindowAndIMEEditBox( CHAT_WINDOW_MODE eMode )
{
	SetChatWindowMode( eMode );

	if( NULL == m_pDLGChatWindow )
		return;


	CKTDGUIListBox* pListBox_Square = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_All" );
	CKTDGUIListBox* pListBox_Party = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Party" );
	CKTDGUIListBox* pListBox_Guild = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Guild" );
	CKTDGUIListBox* pListBox_Personal = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Personal" );
	//{{ 허상형 : [2009/7/17] //	나소드 메가폰 리스트박스 컨트롤
	CKTDGUIListBox* pListBox_Megaphone = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Megaphone" );
	//}} 허상형 : [2009/7/17] //	나소드 메가폰 리스트박스 컨트롤

#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIListBox* pListBox_Relation = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Relation" );
#endif // ADDED_RELATIONSHIP_SYSTEM


	CKTDGUIRadioButton* pRadio_All		= (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonAll" );
	CKTDGUIRadioButton* pRadio_Party	= (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonParty" );
	CKTDGUIRadioButton* pRadio_Guild	= (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonGuild" );
	CKTDGUIRadioButton* pRadio_Personal	= (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonPersonal" );
#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIRadioButton* pRadio_RelationCouple	= static_cast<CKTDGUIRadioButton*> ( m_pDLGChatWindow->GetControl( L"ButtonRelationCouple" ) );
	CKTDGUIRadioButton* pRadio_RelationMarried	= static_cast<CKTDGUIRadioButton*> ( m_pDLGChatWindow->GetControl( L"ButtonRelationMarried") );
#endif // ADDED_RELATIONSHIP_SYSTEM
//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifndef	CHAT_WINDOW_IMPROV
	CKTDGUIStatic* pStatic_ChatMode = (CKTDGUIStatic*) m_pDLGChatWindowEditBox->GetControl( L"Chat_Mode" );
	CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );
#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.10 //	채팅창 개편

	//{{ 허상형 : [2009/7/23] //	탭 변경 시 클릭 지워지게
	if( pListBox_Square != NULL)	pListBox_Square->ClearSelect();
	if( pListBox_Party != NULL)		pListBox_Party->ClearSelect();
	if( pListBox_Guild != NULL)		pListBox_Guild->ClearSelect();
	if( pListBox_Personal != NULL)	pListBox_Personal->ClearSelect();
	if( pListBox_Megaphone != NULL)	pListBox_Megaphone->ClearSelect();
#ifdef ADDED_RELATIONSHIP_SYSTEM
	if( pListBox_Relation != NULL)	pListBox_Relation->ClearSelect();
#endif // ADDED_RELATIONSHIP_SYSTEM
	//}} 허상형 : [2009/7/23] //	탭 변경 시 클릭 지워지게

	switch( GetChatWindowMode() )
	{
	case CWM_TOTAL:
		{
			pListBox_Square->SetEnable( true );
			pListBox_Square->SetShow( true );
			pListBox_Party->SetEnable( false );
			pListBox_Party->SetShow( false );
			pListBox_Guild->SetEnable( false );
			pListBox_Guild->SetShow( false );
			pListBox_Personal->SetShowEnable( false, false );
			//{{ 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부
			pListBox_Megaphone->SetShowEnable( true	, true );
			//}} 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부
			
			pRadio_All->SetChecked( true );
			pRadio_Party->SetChecked( false );
			pRadio_Guild->SetChecked( false );
			pRadio_Personal->SetChecked( false );
#ifdef ADDED_RELATIONSHIP_SYSTEM
			pRadio_RelationMarried->SetChecked( false );
			pRadio_RelationCouple->SetChecked( false );
			pListBox_Relation->SetShowEnable( false, false );
#endif // ADDED_RELATIONSHIP_SYSTEM
			//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifndef	CHAT_WINDOW_IMPROV
			if( pIMEEditBox != NULL && pIMEEditBox->GetShow() == true )
			{
				pStatic_ChatMode->GetPicture(0)->SetShow( true );
				pStatic_ChatMode->GetPicture(1)->SetShow( false );
				pStatic_ChatMode->GetPicture(2)->SetShow( false );
				pStatic_ChatMode->GetPicture(3)->SetShow( false );
			}

			if( NULL != pIMEEditBox )
			{					
				pIMEEditBox->ClearText();
			}
#endif	CHAT_WINDOW_IMPROV
		//}} kimhc // 2010.3.10 //	채팅창 개편

		} break;

	case CWM_PARTY:
		{
			pListBox_Square->SetEnable( false );
			pListBox_Square->SetShow( false );
			pListBox_Party->SetEnable( true );
			pListBox_Party->SetShow( true );
			pListBox_Guild->SetEnable( false );
			pListBox_Guild->SetShow( false );
			pListBox_Personal->SetShowEnable( false, false );
			//{{ 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부
			pListBox_Megaphone->SetShowEnable( true	, true );
			//}} 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부

			pRadio_All->SetChecked( false );
			pRadio_Party->SetChecked( true );
			pRadio_Guild->SetChecked( false );
			pRadio_Personal->SetChecked( false );
#ifdef ADDED_RELATIONSHIP_SYSTEM
			pListBox_Relation->SetShowEnable( false, false );
			pRadio_RelationMarried->SetChecked( false );
			pRadio_RelationCouple->SetChecked( false );
#endif // ADDED_RELATIONSHIP_SYSTEM

		//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifndef	CHAT_WINDOW_IMPROV
			if( pIMEEditBox != NULL && pIMEEditBox->GetShow() == true )
			{
				pStatic_ChatMode->GetPicture(0)->SetShow( false );
				pStatic_ChatMode->GetPicture(1)->SetShow( true );
				pStatic_ChatMode->GetPicture(2)->SetShow( false );
				pStatic_ChatMode->GetPicture(3)->SetShow( false );

			}

			if( NULL != pIMEEditBox )
			{					
				pIMEEditBox->ClearText();
			}
#endif	CHAT_WINDOW_IMPROV
		//}} kimhc // 2010.3.10 //	채팅창 개편

		} break;

	case CWM_GUILD:
		{
			//{{ 허상형 : [2009/9/16] //	길드 버튼 활성화
			
#ifdef	GUILD_MANAGEMENT
			pListBox_Square->SetEnable( false );
			pListBox_Square->SetShow( false );
			pListBox_Party->SetEnable( false );
			pListBox_Party->SetShow( false );
			pListBox_Guild->SetEnable( true );
			pListBox_Guild->SetShow( true );
			pListBox_Personal->SetShowEnable( false, false );
			//{{ 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부
			pListBox_Megaphone->SetShowEnable( true	, true );
			//}} 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부

			pRadio_All->SetChecked( false );
			pRadio_Party->SetChecked( false );
			pRadio_Guild->SetChecked( true );
			pRadio_Personal->SetChecked( false );
#ifdef ADDED_RELATIONSHIP_SYSTEM
			pListBox_Relation->SetShowEnable( false, false );
			pRadio_RelationCouple->SetChecked( false );
			pRadio_RelationMarried->SetChecked( false );
#endif // ADDED_RELATIONSHIP_SYSTEM

		//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifndef	CHAT_WINDOW_IMPROV
			if( pIMEEditBox != NULL && pIMEEditBox->GetShow() == true )
			{
				pStatic_ChatMode->GetPicture(0)->SetShow( false );
				pStatic_ChatMode->GetPicture(1)->SetShow( false );
				pStatic_ChatMode->GetPicture(2)->SetShow( true );
				pStatic_ChatMode->GetPicture(3)->SetShow( false );

			}

			if( NULL != pIMEEditBox )
			{					
				pIMEEditBox->ClearText();
			}
#endif	CHAT_WINDOW_IMPROV
		//}} kimhc // 2010.3.10 //	채팅창 개편

#endif	//	GUILD_MANAGEMENT
			//}} 허상형 : [2009/9/16] //	길드 버튼 활성화

		} break;

	case CWM_PERSONAL:
		{

			pListBox_Square->SetEnable( false );
			pListBox_Square->SetShow( false );
			pListBox_Party->SetEnable( false );
			pListBox_Party->SetShow( false );
			pListBox_Guild->SetEnable( false );
			pListBox_Guild->SetShow( false );
			pListBox_Personal->SetShowEnable( true, true );
			//{{ 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부
			pListBox_Megaphone->SetShowEnable( true	, true );
			//}} 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부

			pRadio_All->SetChecked( false );
			pRadio_Party->SetChecked( false );
			pRadio_Guild->SetChecked( false );
			pRadio_Personal->SetChecked( true );
#ifdef ADDED_RELATIONSHIP_SYSTEM
			pListBox_Relation->SetShowEnable( false, false );
			pRadio_RelationMarried->SetChecked( false );
			pRadio_RelationCouple->SetChecked( false );
#endif // ADDED_RELATIONSHIP_SYSTEM

		//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifndef	CHAT_WINDOW_IMPROV
			if( pIMEEditBox != NULL && pIMEEditBox->GetShow() == true )
			{
				pStatic_ChatMode->GetPicture(0)->SetShow( false );
				pStatic_ChatMode->GetPicture(1)->SetShow( false );
				pStatic_ChatMode->GetPicture(2)->SetShow( false );
				pStatic_ChatMode->GetPicture(3)->SetShow( true );
			}

			if( NULL != m_pDLGChatWindowEditBox )
			{
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );	// kimhc // fix!! 위에 선언되어 있어서 필요없을듯
				if( NULL != pIMEEditBox )
				{
					wstring wstrlastWhiperUser = L"/w ";
					if(m_wstrLastWhiperNickName.empty() == false)
					{
						wstrlastWhiperUser += m_wstrLastWhiperNickName;
						wstrlastWhiperUser += L" ";
					}					
					pIMEEditBox->SetText( wstrlastWhiperUser.c_str() );
				}
			}
#endif	CHAT_WINDOW_IMPROV
		//}} kimhc // 2010.3.10 //	채팅창 개편

		} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CWM_RELATION_COUPLE :
	case CWM_RELATION_MARRIED :
		{

#ifdef	GUILD_MANAGEMENT
			pListBox_Square->SetEnable( false );
			pListBox_Square->SetShow( false );
			pListBox_Party->SetEnable( false );
			pListBox_Party->SetShow( false );
			pListBox_Guild->SetEnable( false );
			pListBox_Guild->SetShow( false );
			pListBox_Personal->SetShowEnable( false, false );
			//{{ 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부
			pListBox_Megaphone->SetShowEnable( true	, true );
			//}} 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부

			pRadio_All->SetChecked( false );
			pRadio_Party->SetChecked( false );
			pRadio_Guild->SetChecked( false );
			pRadio_Personal->SetChecked( false );
			
			pListBox_Relation->SetShowEnable( true, true );

				
			if ( NULL != g_pData->GetRelationshipManager() && 
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() ) 
			{
				switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
				{
				case SEnum::RT_COUPLE :
					pRadio_RelationMarried->SetEnable( false );
					pRadio_RelationMarried->SetShow( false );
					pRadio_RelationCouple->SetEnable( true );
					pRadio_RelationCouple->SetShow( true );
					break;

				case SEnum::RT_MARRIED :				
					pRadio_RelationMarried->SetEnable( true );
					pRadio_RelationMarried->SetShow( true );
					pRadio_RelationCouple->SetEnable( false );
					pRadio_RelationCouple->SetShow( false );
					break;
				}
			}

			//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifndef	CHAT_WINDOW_IMPROV
			if( pIMEEditBox != NULL && pIMEEditBox->GetShow() == true )
			{
				pStatic_ChatMode->GetPicture(0)->SetShow( false );
				pStatic_ChatMode->GetPicture(1)->SetShow( false );
				pStatic_ChatMode->GetPicture(2)->SetShow( true );
				pStatic_ChatMode->GetPicture(3)->SetShow( false );

			}

			if( NULL != pIMEEditBox )
			{					
				pIMEEditBox->ClearText();
			}
#endif	CHAT_WINDOW_IMPROV
			//}} kimhc // 2010.3.10 //	채팅창 개편

#endif	//	GUILD_MANAGEMENT
			//}} 허상형 : [2009/9/16] //	길드 버튼 활성화

		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	case CWM_MEGAPHONE:
		{

			pListBox_Square->SetEnable( false );
			pListBox_Square->SetShow( false );
			pListBox_Party->SetEnable( false );
			pListBox_Party->SetShow( false );
			pListBox_Guild->SetEnable( false );
			pListBox_Guild->SetShow( false );
			pListBox_Personal->SetShowEnable( false, false );
			pListBox_Megaphone->SetShowEnable( true, true );


			pRadio_All->SetChecked( false );
			pRadio_Party->SetChecked( false );
			pRadio_Guild->SetChecked( false );
			pRadio_Personal->SetChecked( false );
#ifdef ADDED_RELATIONSHIP_SYSTEM
			pListBox_Relation->SetShowEnable( false, false );
			pRadio_RelationCouple->SetChecked( false );
			pRadio_RelationMarried->SetChecked( false );
#endif // ADDED_RELATIONSHIP_SYSTEM

			//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifndef	CHAT_WINDOW_IMPROV
			//	전체 모드 아이콘이 뜨도록
			if( pIMEEditBox != NULL && pIMEEditBox->GetShow() == true )
			{
				pStatic_ChatMode->GetPicture(0)->SetShow( true );
				pStatic_ChatMode->GetPicture(1)->SetShow( false );
				pStatic_ChatMode->GetPicture(2)->SetShow( false );
				pStatic_ChatMode->GetPicture(3)->SetShow( false );
			}

			if( NULL != m_pDLGChatWindowEditBox )
			{
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" ); // kimhc // fix!! // 위에 선언되어 있어서 필요없을듯
				if( NULL != pIMEEditBox )
				{
					pIMEEditBox->ClearText();
				}
			}
#endif	CHAT_WINDOW_IMPROV
			//}} kimhc // 2010.3.10 //	채팅창 개편

		} break;
		//}} 허상형 : [2009/7/17] //
	}

//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
		ChangeIMEEditBox( GetChatWindowMode() );
#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.10 //	채팅창 개편

	const float MAGIC_CHAT_SHOW_TIME = 50.0f;

	//{{ 허상형 : [2009/7/24] //	탭 이동시 메시지 보이게 설정
	pListBox_Square->SetShowAllItems( true );
	pListBox_Square->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
	pListBox_Party->SetShowAllItems( true );
	pListBox_Party->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
	pListBox_Guild->SetShowAllItems( true );
	pListBox_Guild->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
	pListBox_Personal->SetShowAllItems( true );
	pListBox_Personal->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );


	//{{ 허상형 : [2009/7/24] //
	pListBox_Megaphone->SetShowAllItems( true );
	pListBox_Megaphone->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );

#ifdef ADDED_RELATIONSHIP_SYSTEM
	pListBox_Relation->SetShowAllItems( true );
	pListBox_Relation->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
#endif // ADDED_RELATIONSHIP_SYSTEM
	//}} 허상형 : [2009/7/24] //

	//추가된 팝업 메뉴에 대한 설정
	if( true == m_pDLGTabMenu->GetShow() )
		m_pDLGTabMenu->SetShow(false);
}

void CX2ChatBox::HideChatLog()
{
	if( NULL == m_pDLGChatWindow )
		return;

	CKTDGUIListBox* pListBox_All = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_All" );
	CKTDGUIListBox* pListBox_Party = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Party" );
	CKTDGUIListBox* pListBox_Guild = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Guild" );
	CKTDGUIListBox* pListBox_Personal = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Personal" );
	//{{ 허상형 : [2009/7/17] //	나소드 메가폰 관련

	CKTDGUIListBox* pListBox_Megaphone = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Megaphone" );

	//}} 허상형 : [2009/7/17] //

#ifdef ADDED_RELATIONSHIP_SYSTEM

	CKTDGUIListBox* pListBox_Relation = static_cast <CKTDGUIListBox*> ( m_pDLGChatWindow->GetControl( L"ListBox_Relation" ) );

#endif // ADDED_RELATIONSHIP_SYSTEM



	pListBox_All->SetShowAllItems( false );
	pListBox_Party->SetShowAllItems( false );
	pListBox_Guild->SetShowAllItems( false );
	pListBox_Personal->SetShowAllItems( false );

#ifdef ADDED_RELATIONSHIP_SYSTEM
	pListBox_Relation->SetShowAllItems( false );
#endif // ADDED_RELATIONSHIP_SYSTEM

	//{{ 허상형 : [2009/7/17] //	나소드 메가폰 관련

	pListBox_Megaphone->SetShowAllItems( false );

	//}} 허상형 : [2009/7/17] //
}

UidType CX2ChatBox::GetWhisperChatTargetUID( const WCHAR* wszNickName, bool bParty /*= true*/ )
{
	UidType iTargetUnitUID = -1;
	wstring wstrTargetUnitNickName = wszNickName;

	if( true == bParty )
	{
		CX2PartyManager::PartyMemberData* pPartyUserInfo = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( wstrTargetUnitNickName );
		if( NULL == pPartyUserInfo )
		{
			return -1;
		}
		else
		{
			iTargetUnitUID = pPartyUserInfo->m_iUnitUID;
		}

		if( iTargetUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() ) 
		{
			return -1;
		}
	}
	else
	{
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
			{
				CX2SquareUnit* pSquareUnit = g_pTFieldGame->GetSquareUnitByNickName( wstrTargetUnitNickName.c_str() );

				if( NULL == pSquareUnit )
				{
					return -1;
				}
				else
				{
					iTargetUnitUID = pSquareUnit->GetUID();
				}

				if( iTargetUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() ) 
				{
					return -1;
				}

			} break;
		case CX2Main::XS_SQUARE_GAME:
			{
				CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByNickName( wstrTargetUnitNickName.c_str() );

				if( NULL == pSquareUnit )
				{
					return -1;
				}
				else
				{
					iTargetUnitUID = pSquareUnit->GetUID();
				}

				if( iTargetUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() ) 
				{
					return -1;
				}

			} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
		case CX2Main::XS_WEDDING_GAME:
			{
				CX2SquareUnit* pSquareUnit = g_pWeddingGame->GetSquareUnitByNickName( wstrTargetUnitNickName.c_str() );

				if( NULL == pSquareUnit )
				{
					return -1;
				}
				else
				{
					iTargetUnitUID = pSquareUnit->GetUID();
				}

				if( iTargetUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() ) 
				{
					return -1;
				}

			} break;
#endif // ADDED_RELATIONSHIP_SYSTEM

		case CX2Main::XS_PVP_ROOM:
		case CX2Main::XS_DUNGEON_ROOM:
		case CX2Main::XS_PVP_GAME:
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_TRAINING_GAME:
			{
				CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByNickName( wstrTargetUnitNickName.c_str() );
				if( NULL != pSlotData )
				{
					if( true == pSlotData->IsMyUnit() )
						return -1;


					iTargetUnitUID = pSlotData->m_pUnit->GetUID();
				}

			} break;

		}
	}

	return iTargetUnitUID;
}


wstring CX2ChatBox::GetUnitNickName( UidType iUnitUID )
{
	wstring wstrNickName = L"";

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			if( NULL != g_pTFieldGame &&
				NULL != g_pTFieldGame->GetSquareUnitByUID( iUnitUID) )
			{
				wstrNickName = g_pTFieldGame->GetSquareUnitByUID( iUnitUID )->GetUnit()->GetNickName();
			}

		} break;
	case CX2Main::XS_SQUARE_GAME:
		{
			if( NULL != g_pSquareGame &&
				NULL != g_pSquareGame->GetSquareUnitByUID( iUnitUID) )
			{
				wstrNickName = g_pSquareGame->GetSquareUnitByUID( iUnitUID )->GetUnit()->GetNickName();
			}

		} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
		{
			if( NULL != g_pWeddingGame &&
				NULL != g_pWeddingGame->GetSquareUnitByUID( iUnitUID) )
			{
				wstrNickName = g_pWeddingGame->GetSquareUnitByUID( iUnitUID )->GetUnit()->GetNickName();
			}
		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM

	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_DUNGEON_ROOM:
		{
			if ( NULL == g_pX2Room )
				break;

			if ( NULL == g_pX2Room->GetSlotDataByUnitUID( iUnitUID ) )
				break;

			if ( NULL == g_pX2Room->GetSlotDataByUnitUID( iUnitUID )->m_pUnit )
				break;

			if ( NULL == g_pX2Room->GetSlotDataByUnitUID( iUnitUID )->m_pUnit->GetNickName() )
				break;

			wstrNickName = g_pX2Room->GetSlotDataByUnitUID( iUnitUID )->m_pUnit->GetNickName();

		} break;
	}


	if( 0 == wstrNickName.length() )
	{
		// 사제 리스트를 찾아본다
		if( NULL != g_pMain->GetTutorSystem() )
		{
			const CX2TutorSystem::TutorUnit* pTutorUnit = g_pMain->GetTutorSystem()->GetTutorUnit( iUnitUID );
			if( NULL != pTutorUnit )
			{
				wstrNickName = pTutorUnit->m_wstrNickName;
			}
		}
	}

	return wstrNickName;
}


UidType CX2ChatBox::GetUnitUIDByNickName( const WCHAR* wszNickName )
{
	UidType unitUID = -1;

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			if( NULL != g_pTFieldGame &&
				NULL != g_pTFieldGame->GetSquareUnitByNickName( wszNickName ) )
			{
				unitUID = g_pTFieldGame->GetSquareUnitByNickName( wszNickName )->GetUID();
			}

		} break;
	case CX2Main::XS_SQUARE_GAME:
		{
			if( NULL != g_pSquareGame &&
				NULL != g_pSquareGame->GetSquareUnitByNickName( wszNickName ) )
			{
				unitUID = g_pSquareGame->GetSquareUnitByNickName( wszNickName )->GetUID();
			}
		} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
		{
			if( NULL != g_pWeddingGame &&
				NULL != g_pWeddingGame->GetSquareUnitByNickName( wszNickName ) )
			{
				unitUID = g_pWeddingGame->GetSquareUnitByNickName( wszNickName )->GetUID();
			}
		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM

	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_DUNGEON_ROOM:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_DUNGEON_GAME:
		{
			if ( NULL == g_pX2Room )
				break;

			if ( NULL == g_pX2Room->GetSlotDataByNickName( wszNickName ) )
				break;

			if ( NULL == g_pX2Room->GetSlotDataByNickName( wszNickName )->m_pUnit )
				break;

			unitUID = g_pX2Room->GetSlotDataByNickName( wszNickName )->m_pUnit->GetUID();

		} break;
	}

	return unitUID;

}


void CX2ChatBox::SetFocus()
{
	CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );
	if( NULL != pIMEEditBox )
		pIMEEditBox->RequestFocus();
}

bool CX2ChatBox::GetFocus()
{
	if ( m_pDLGChatWindowEditBox != NULL )
	{
		CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );
		if ( pIMEEditBox != NULL )
		{
			return pIMEEditBox->GetHaveFocusIn();
		}
	}

	return false;
}

bool CX2ChatBox::OnOpenWhisperChatMode()
{
	if( NULL == m_pDLGChatWindow )
		return false;

	// 내 자신이거나 메시지를 보낼 아이디가 없는 경우 빠져나감
	if( g_pData->GetMyUser()->GetSelectUnit()->GetNickName() == m_wstrPickedUserNickName || m_wstrPickedUserNickName == L"" )
	{
		return true;
	}

	CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );
	if( NULL == pIMEEditBox )
		return false;


	wstring wstrText = L"/w ";
	wstrText += m_wstrPickedUserNickName;
	wstrText += L" ";


	if( false == m_bOpenChatWindow )
	{
		OpenChatWindow();
	}

	UnHideChatEditBox();

	pIMEEditBox->SetText( wstrText.c_str() );
	pIMEEditBox->RequestFocus();

	// 09. 05. 13김정협 언어가 국어일경우만 아래함수실행하도록 버그수정
	if(pIMEEditBox->IsKorean())
	{
#ifdef CHAT_KOREAN_REMOVE
		g_pMain->ChangeKoreanAndEnglishKey( false );
#else CHAT_KOREAN_REMOVE
		g_pMain->ChangeKoreanAndEnglishKey( true );
#endif CHAT_KOREAN_REMOVE
	}

	return true;
}

bool CX2ChatBox::SetChatEditText( const WCHAR* wszText )
{
	if( NULL == m_pDLGChatWindow )
		return false;

	CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" );
	if( NULL == pIMEEditBox )
		return false;

	if( NULL == wszText )
	{
		pIMEEditBox->ClearText();
	}
	else
	{		
		pIMEEditBox->SetText( wszText );
		pIMEEditBox->RequestFocus();
	}

	return true;
}

void CX2ChatBox::SetChatDialogPos( float x, float y )
{
	if( NULL != m_pDLGChatWindow )
		m_pDLGChatWindow->SetPos( D3DXVECTOR2( x, y ) );
}


void CX2ChatBox::SetScrollBarEndPos()
{
	if( NULL == m_pDLGChatWindow )
		return;


	CKTDGUIListBox* pListBox_All = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_All" );
	CKTDGUIListBox* pListBox_Party = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Party" );
	CKTDGUIListBox* pListBox_Guild = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Guild" );
	CKTDGUIListBox* pListBox_Personal = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Personal" );
	CKTDGUIListBox* pListBox_Megaphone = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Megaphone" );

#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIListBox* pListBox_Relation = static_cast <CKTDGUIListBox*> ( m_pDLGChatWindow->GetControl( L"ListBox_Relation" ) );
#endif // ADDED_RELATIONSHIP_SYSTEM

	if( NULL != pListBox_All )
		pListBox_All->SetScrollBarEndPos();
	if( NULL != pListBox_Party )
		pListBox_Party->SetScrollBarEndPos();
	if( NULL != pListBox_Guild )
		pListBox_Guild->SetScrollBarEndPos();
	if( NULL != pListBox_Personal )
		pListBox_Personal->SetScrollBarEndPos();
	if( NULL != pListBox_Megaphone )
		pListBox_Megaphone->SetScrollBarEndPos();

#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( NULL != pListBox_Relation )
		pListBox_Relation->SetScrollBarEndPos();
#endif // ADDED_RELATIONSHIP_SYSTEM

}

void CX2ChatBox::ClearShortCutKeyProcess()
{
	// 채팅 입력후 shortcut key가 작동하지 않도록 키보드 입력 데이터를 지운다
	SET_KEYLOCK( GA_ATTACK_FAST, TRUE );
	SET_KEYLOCK( GA_ATTACK_STRONG, TRUE );

	SET_KEYLOCK( GA_SKILL1, TRUE );
	SET_KEYLOCK( GA_SKILL2, TRUE );
	SET_KEYLOCK( GA_SKILL3, TRUE );
	SET_KEYLOCK( GA_SKILL4, TRUE );
	SET_KEYLOCK( GA_SKILL_EX1, TRUE );
	SET_KEYLOCK( GA_SKILL_EX2, TRUE );
	SET_KEYLOCK( GA_SKILL_EX3, TRUE );
	SET_KEYLOCK( GA_SKILL_EX4, TRUE );

//	SET_KEYLOCK( GA_SCREENSHOT_MODE, TRUE );
	SET_KEYLOCK( GA_SLOT_CHANGE, TRUE );
	SET_KEYLOCK( GA_PVE, TRUE );
	SET_KEYLOCK( GA_PVP, TRUE );
	SET_KEYLOCK( GA_ZOOMIN, TRUE );
	SET_KEYLOCK( GA_ZOOMOUT, TRUE );
	SET_KEYLOCK( GA_QUEST, TRUE );
	SET_KEYLOCK( GA_MAP, TRUE );
	SET_KEYLOCK( GA_COMMUNITY, TRUE );
	SET_KEYLOCK( GA_INFO, TRUE );
	SET_KEYLOCK( GA_INVENTORY, TRUE );
	SET_KEYLOCK( GA_OPTION, TRUE );
	SET_KEYLOCK( GA_PET, TRUE );
	SET_KEYLOCK( GA_GUILD, TRUE );
	SET_KEYLOCK( GA_SKILL_TREE, TRUE );
	SET_KEYLOCK( GA_NAME, TRUE );
	SET_KEYLOCK( GA_FRIEND, TRUE );
#ifdef RIDING_SYSTEM
	SET_KEYLOCK( GA_RIDING, TRUE );
#endif //RIDING_SYSTEM

#ifdef ADDED_RELATIONSHIP_SYSTEM
	SET_KEYLOCK( GA_RELATION, TRUE );
#endif //ADDED_RELATIONSHIP_SYSTEM

	SET_KEYLOCK( GA_QUICKSLOT1, TRUE );
	SET_KEYLOCK( GA_QUICKSLOT2, TRUE );
	SET_KEYLOCK( GA_QUICKSLOT3, TRUE );
	SET_KEYLOCK( GA_QUICKSLOT4, TRUE );
	SET_KEYLOCK( GA_QUICKSLOT5, TRUE );
	SET_KEYLOCK( GA_QUICKSLOT6, TRUE );

	SET_KEYLOCK( GA_SIT, TRUE );

#ifdef SERV_ADD_WARP_BUTTON
	SET_KEYLOCK( GA_WARP, TRUE );
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_KEY_MAPPING_INT
	SET_KEYLOCK( GA_RETURN, TRUE );
	SET_KEYLOCK( GA_ESCAPE, TRUE );
	SET_KEYLOCK( GA_PARTYREADY, TRUE );
#endif // SERV_KEY_MAPPING_INT

#if 0 
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_Q);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_I);	
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_S);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_P);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_M);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_N);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_T);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_K);

	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_Z);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_X);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_W);	
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_A);	
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_D);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_C);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_O);

	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_1);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_2);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_3);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_4);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_5);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_6);
#endif
}

void CX2ChatBox::SetChatWindowEnlarged( bool bEnlarged )
{
    //{{ seojt // 2009-8-21, 11:58
    if( m_pDLGChatWindow == NULL )
    {
        return;
    }//if
    //}} seojt // 2009-8-21, 11:58

	m_bChatWindowEnlarged = bEnlarged;
	//채팅창 확장이 없어졌기 떄문에 형식 변경
	CKTDGUIStatic* pStatic_BG_Common = (CKTDGUIStatic*) m_pDLGChatWindow->GetControl( L"Chat_Box_Common" );
	CKTDGUIStatic* pStatic_BG_Megaphone = (CKTDGUIStatic*) m_pDLGChatWindow->GetControl( L"Chat_Box_Megaphone" );
	if( NULL != pStatic_BG_Common)
	{
		pStatic_BG_Common->SetShowEnable( true, true );
	}
	if( NULL != pStatic_BG_Megaphone )
	{
		pStatic_BG_Megaphone->SetShowEnable( true, true );
	}	
}

void CX2ChatBox::SetPickedUser( UidType iPickedUserUID, wstring strPickedUserNickName )
{
	m_iPickedUserUID = iPickedUserUID;
	m_wstrPickedUserNickName = strPickedUserNickName;
}

bool CX2ChatBox::GetMouseOverChatBox()
{
	KTDXPROFILE();

    //{{ seojt // 2009-8-21, 11:59
    if( m_pDLGChatWindow == NULL )
    {
        return false;
    }//if
    //}} seojt // 2009-8-21, 11:59

	bool bResult = false;
	bResult = m_pDLGChatWindow->GetIsMouseOver();
	return bResult;
}

void CX2ChatBox::SetChatBoxLayerUp(bool val)
{
	if(val == true)
	{
		//m_pDLGChatWindowButton->SetLayer(XDL_MESSENGER);
		m_pDLGChatWindow->SetLayer(XDL_MESSENGER);
		m_pDLGChatWindowHelper->SetLayer(XDL_MESSENGER);
		m_pDLGEmotionList->SetLayer(XDL_MESSENGER);
		//m_pDLGChatWindowEditBox->SetLayer(XDL_MESSENGER);
	}
	else
	{
		//m_pDLGChatWindowButton->SetLayer(XDL_MESSENGER);
		m_pDLGChatWindow->SetLayer(XDL_NORMAL);
		m_pDLGChatWindowHelper->SetLayer(XDL_NORMAL);
		m_pDLGEmotionList->SetLayer(XDL_NORMAL);
		//m_pDLGChatWindowEditBox->SetLayer(XDL_NORMAL);
	}	
}

//{{ kimhc // 채팅창에 있는 모든 글을 지움 // 2009-06-16
void CX2ChatBox::ClearAllChatLog()
{
    //{{ seojt // 2009-8-21, 11:59
    if( m_pDLGChatWindow == NULL )
    {
        return;
    }//if
    //}} seojt // 2009-8-21, 11:59

	m_vecQueuedChatLog.clear();
	CKTDGUIListBox* pListBoxAll			= (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_All" );
	pListBoxAll->RemoveAllItems();

	CKTDGUIListBox* pListBoxParty		= (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Party" );
	pListBoxParty->RemoveAllItems();

	CKTDGUIListBox* pListBoxGuild		= (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Guild" );
	pListBoxGuild->RemoveAllItems();

	CKTDGUIListBox* pListBoxPersonal	= (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Personal" );
	pListBoxPersonal->RemoveAllItems();

#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIListBox* pListBoxRelation	= static_cast <CKTDGUIListBox*> ( m_pDLGChatWindow->GetControl( L"ListBox_Relation" ) );
	pListBoxRelation->RemoveAllItems();
#endif // ADDED_RELATIONSHIP_SYSTEM

	//{{ 허상형 : [2009/11/3] //	메가폰 메세지는 살려둠
//	CKTDGUIListBox* pListBoxMegaPhone	= (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Megaphone" );
//	pListBoxMegaPhone->RemoveAllItems();
}
//}} kimhc // 채팅창에 있는 모든 글을 지움 // 2009-06-16

#ifdef UNIT_EMOTION
bool CX2ChatBox::IsEmotionID( const wstring &wstrChatMsg ) const
{	
	// 이모션 명령어
	if(wstrChatMsg.compare( GET_STRING( STR_ID_2501 ) ) == 0)
	{
		return true;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_2502 ) ) == 0)
	{
		return true;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_2503 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2504 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2505 ) ) == 0)
	{
		return true;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_2506 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2507 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2508 ) ) == 0)
	{
		return true;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_2509 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2510 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2511 ) ) == 0)
	{
		return true;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_2512 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2513 ) ) == 0)
	{
		return true;
	}
	//else if(wstrChatMsg.compare(L"/축하") == 0 || wstrChatMsg.compare(L"/박수") == 0)
	//{
	//	return true;
	//}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_2514 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2515 ) ) == 0)
	{
		return true;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_11545 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_11547 ) ) == 0)
	{
		return true;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_15749 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_15750 ) ) == 0)
	{
		return true;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_17495 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_17496 ) ) == 0)
	{
		return true;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_18263 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_18264 ) ) == 0)
	{
		return true;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_25166 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_25167 ) ) == 0 )
	{		
		return true;
	}
#ifdef CRAYONPOP_SECOND_EMOTION
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_27031 ) ) == 0 )
	{
		return true;
	}
#endif // CRAYONPOP_SECOND_EMOTION

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
	else if(wstrChatMsg.compare( L"/꾸리스마스" ) == 0 )
	{
		return true;
	}
#endif // CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨

	//else if(wstrChatMsg.compare(L"/수락") == 0 || wstrChatMsg.compare(L"/그래") == 0)
	//{
	//	return true;
	//}
	//else if(wstrChatMsg.compare(L"/승리") == 0)
	//{
	//	return true;
	//}
	//else if(wstrChatMsg.compare(L"/준비") == 0 || wstrChatMsg.compare(L"/전투준비") == 0)
	//{
	//	return true;
	//}
	//else if(wstrChatMsg.compare(L"/춤") == 0)
	//{
	//	return true;
	//}
	//else if(wstrChatMsg.compare(L"/어이") == 0 || wstrChatMsg.compare(L"/황당") == 0)
	//{
	//	return true;
	//}				
	//else if(wstrChatMsg.compare(L"/고백") == 0)
	//{
	//	return true;
	//}
	//else if(wstrChatMsg.compare(L"/부끄") == 0 || wstrChatMsg.compare(L"/아잉") == 0)
	//{
	//	return true;
	//}				
	//else if(wstrChatMsg.compare(L"/좌절") == 0 || wstrChatMsg.compare(L"/OTL") == 0)
	//{
	//	return true;
	//}				
	//else if(wstrChatMsg.compare(L"/도발") == 0)
	//{
	//	return true;
	//}

	return false;
}


CX2Unit::EMOTION_TYPE CX2ChatBox::GetEmotionID( const wstring &wstrChatMsg ) const
{
	CX2Unit::EMOTION_TYPE eResult = CX2Unit::ET_NONE;
	// 이모션 명령어
	if(wstrChatMsg.compare( GET_STRING( STR_ID_2501 ) ) == 0)
	{
		eResult = CX2Unit::ET_SITREADY;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_2502 ) ) == 0)
	{
		eResult = CX2Unit::ET_STANDUP;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_2503 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2504 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2505 ) ) == 0 ||
		wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2516 ) ) != NULL || wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2517 ) ) != NULL || wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2518 ) ) != NULL ||
		wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2519 ) ) != NULL || wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2520 ) ) != NULL)
	{
		eResult = CX2Unit::ET_HELLO;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_2506 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2508 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2507 ) ) == 0 ||
		wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2521 ) ) != NULL || wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2522 ) ) != NULL || wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2523 ) ) != NULL ||
		wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2524 ) ) != NULL || wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2525 ) ) != NULL || wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2526 ) ) != NULL ||
		wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2527 ) ) != NULL || wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2528 ) ) != NULL || wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2529 ) ) != NULL ||
		wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2530 ) ) != NULL || wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2531 ) ) != NULL )
	{
		eResult = CX2Unit::ET_SMILE;
	}
	else if( wstrChatMsg.compare( GET_STRING( STR_ID_2509 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2510 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2511 ) ) == 0 ||
		wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2532 ) ) != NULL || wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2533 ) ) != NULL || 
		wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2534 ) ) != NULL ||	wcsstr(wstrChatMsg.c_str(), GET_STRING( STR_ID_2535 ) ) != NULL )
	{
		eResult = CX2Unit::ET_SAD;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_2512 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2513 ) ) == 0 ||
		wcsstr(wstrChatMsg.c_str(),  GET_STRING( STR_ID_2536 ) ) != NULL)
	{
		eResult = CX2Unit::ET_ANGRY;
	}
	//else if(wstrChatMsg.compare(L"/축하") == 0 || wstrChatMsg.compare(L"/박수") == 0 ||
	//	wcsstr(wstrChatMsg.c_str(), L"축하") != NULL || wcsstr(wstrChatMsg.c_str(), L"ㅊㅋ") != NULL || wcsstr(wstrChatMsg.c_str(), L"추카") != NULL)
	//{
	//	eResult = CX2Unit::ET_NONE;
	//}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_11545 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_11547 ) ) == 0 ||
		wcsstr(wstrChatMsg.c_str(),  GET_STRING( STR_ID_11546 ) ) != NULL || wcsstr(wstrChatMsg.c_str(),  GET_STRING( STR_ID_11548 ) ) != NULL )
	{		
		eResult = CX2Unit::ET_EMOTION_AVATAR1;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_15749 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_15750 ) ) == 0 ||
		wcsstr(wstrChatMsg.c_str(),  GET_STRING( STR_ID_15765 ) ) != NULL || wcsstr(wstrChatMsg.c_str(),  GET_STRING( STR_ID_15766 ) ) != NULL )
	{		
		eResult = CX2Unit::ET_EMOTION_AVATAR2;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_17495 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_17496 ) ) == 0 ||
		wcsstr(wstrChatMsg.c_str(),  GET_STRING( STR_ID_17497 ) ) != NULL || wcsstr(wstrChatMsg.c_str(),  GET_STRING( STR_ID_17498 ) ) != NULL )
	{		
		eResult = CX2Unit::ET_EMOTION_AVATAR3;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_18263 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_18264 ) ) == 0 )
	{		
		eResult = CX2Unit::ET_EMOTION_AVATAR4;
	}
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_25166 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_25167 ) ) == 0 )
	{		
		eResult = CX2Unit::ET_EMOTION_AVATAR5;
	}
#ifdef CRAYONPOP_SECOND_EMOTION
	else if(wstrChatMsg.compare( GET_STRING( STR_ID_27031 ) ) == 0 )
	{
		eResult = CX2Unit::ET_EMOTION_AVATAR6;
	}
#endif // CRAYONPOP_SECOND_EMOTION

#ifdef CRAYONPOP_SECOND_EMOTION
	else if(wstrChatMsg.compare( L"/꾸리스마스" ) == 0 )
	{
		eResult = CX2Unit::ET_EMOTION_AVATAR7;
	}
#endif // CRAYONPOP_SECOND_EMOTION


	else if(wstrChatMsg.compare( GET_STRING( STR_ID_2514 ) ) == 0 || wstrChatMsg.compare( GET_STRING( STR_ID_2515 ) ) == 0 ||
		wcsstr(wstrChatMsg.c_str(),  GET_STRING( STR_ID_2537 ) ) != NULL)
	{
		eResult = CX2Unit::ET_NO;
	}
	//else if(wstrChatMsg.compare(L"/수락") == 0 || wstrChatMsg.compare(L"/그래") == 0 ||
	//	wcsstr(wstrChatMsg.c_str(), L"ㅇㅇ") != NULL)
	//{
	//	eResult = CX2Unit::ET_NONE;
	//}
	//else if(wstrChatMsg.compare(L"/승리") == 0)
	//{
	//	eResult = CX2Unit::ET_NONE;
	//}
	//else if(wstrChatMsg.compare(L"/준비") == 0 || wstrChatMsg.compare(L"/전투준비") == 0 ||
	//	wcsstr(wstrChatMsg.c_str(), L"ㄱㄱ") != NULL)
	//{
	//	eResult = CX2Unit::ET_NONE;
	//}
	//else if(wstrChatMsg.compare(L"/춤") == 0)
	//{
	//	eResult = CX2Unit::ET_NONE;
	//}
	//else if(wstrChatMsg.compare(L"/어이") == 0 || wstrChatMsg.compare(L"/황당") == 0)
	//{
	//	eResult = CX2Unit::ET_NONE;
	//}				
	//else if(wstrChatMsg.compare(L"/고백") == 0)
	//{
	//	eResult = CX2Unit::ET_NONE;
	//}
	//else if(wstrChatMsg.compare(L"/부끄") == 0 || wstrChatMsg.compare(L"/아잉") == 0)
	//{
	//	eResult = CX2Unit::ET_NONE;
	//}				
	//else if(wstrChatMsg.compare(L"/좌절") == 0 || wstrChatMsg.compare(L"/OTL") == 0)
	//{
	//	eResult = CX2Unit::ET_NONE;
	//}				
	//else if(wstrChatMsg.compare(L"/도발") == 0)
	//{
	//	eResult = CX2Unit::ET_NONE;
	//}

	return eResult;
}

wstring CX2ChatBox::GetEmotionName( const wstring &wstrChatMsg) const
{
	wstring wstrResult = L"";
	// 이모션 명령어
	switch(GetEmotionID(wstrChatMsg))
	{
	case CX2Unit::ET_NONE:
	case CX2Unit::ET_SITWAIT:
		break;
	case CX2Unit::ET_SITREADY:
		wstrResult = L"Emotion_SitReady";
		break;
	case CX2Unit::ET_STANDUP:
		wstrResult = L"Emotion_StandUp";
		break;
	case CX2Unit::ET_ANGRY:
		wstrResult = L"Emotion_Angry";
		break;
	case CX2Unit::ET_HELLO:
		wstrResult = L"Emotion_Hello";
		break;
	case CX2Unit::ET_NO:
		wstrResult = L"Emotion_No";
		break;
	case CX2Unit::ET_SAD:
		wstrResult = L"Emotion_Sad";
		break;
	case CX2Unit::ET_SMILE:
		wstrResult = L"Emotion_Smile";
		break;
	case CX2Unit::ET_EMOTION_AVATAR1:
		wstrResult = L"Emotion_BIGBANG";
		break;
	case CX2Unit::ET_EMOTION_AVATAR2:
		wstrResult = L"Emotion_DeepBow";
		break;
	case CX2Unit::ET_EMOTION_AVATAR3:
		wstrResult = L"Emotion_APINK";
		break;
	case CX2Unit::ET_EMOTION_AVATAR4:
		wstrResult = L"Emotion_APINK_LOVE";
		break;
	case CX2Unit::ET_EMOTION_AVATAR5:
		wstrResult = L"Emotion_CRAYONPOP";
		break;
#ifdef CRAYONPOP_SECOND_EMOTION // 김태환
	case CX2Unit::ET_EMOTION_AVATAR6:	// CRAYONPOP 한벌
		wstrResult = L"Emotion_BbaBbaBba";
		break;
#endif // CRAYONPOP_SECOND_EMOTION

	default:
		break;
	}

	return wstrResult;
}

bool CX2ChatBox::GetEmotionIdAndName(CX2Unit::EMOTION_TYPE &eId, wstring &wstrName, const wstring &wstrChatMsg)
{	
	eId = CX2Unit::ET_NONE;
	wstrName = L"";

	switch(GetEmotionID(wstrChatMsg))
	{
	case CX2Unit::ET_NONE:
	case CX2Unit::ET_SITWAIT:
		break;
	case CX2Unit::ET_SITREADY:
		wstrName = L"Emotion_SitReady";
		eId = CX2Unit::ET_SITREADY;
		return true;
	case CX2Unit::ET_STANDUP:
		wstrName = L"Emotion_StandUp";
		eId = CX2Unit::ET_STANDUP;
		return true;
	case CX2Unit::ET_ANGRY:
		wstrName = L"Emotion_Angry";
		eId = CX2Unit::ET_ANGRY;
		return true;
	case CX2Unit::ET_HELLO:
		wstrName = L"Emotion_Hello";
		eId = CX2Unit::ET_HELLO;
		return true;
	case CX2Unit::ET_NO:
		wstrName = L"Emotion_No";
		eId = CX2Unit::ET_NO;
		return true;
	case CX2Unit::ET_SAD:
		wstrName = L"Emotion_Sad";
		eId = CX2Unit::ET_SAD;
		return true;
	case CX2Unit::ET_SMILE:
		wstrName = L"Emotion_Smile";
		eId = CX2Unit::ET_SMILE;
		return true;
	case CX2Unit::ET_EMOTION_AVATAR1:
		wstrName = L"Emotion_BIGBANG";
		eId = CX2Unit::ET_EMOTION_AVATAR1;
		return true;
	case CX2Unit::ET_EMOTION_AVATAR2:
		wstrName = L"Emotion_DeepBow";
		eId = CX2Unit::ET_EMOTION_AVATAR2;
		return true;
	case CX2Unit::ET_EMOTION_AVATAR3:
		wstrName = L"Emotion_APINK";
		eId = CX2Unit::ET_EMOTION_AVATAR3;
		return true;
	case CX2Unit::ET_EMOTION_AVATAR4:
		wstrName = L"Emotion_APINK_LOVE";
		eId = CX2Unit::ET_EMOTION_AVATAR4;
		return true;
	case CX2Unit::ET_EMOTION_AVATAR5:
		wstrName = L"Emotion_CRAYONPOP";
		eId = CX2Unit::ET_EMOTION_AVATAR5;
		return true;

#ifdef CRAYONPOP_SECOND_EMOTION // 김태환
	case CX2Unit::ET_EMOTION_AVATAR6:
		wstrName = L"Emotion_BbaBbaBba";
		eId = CX2Unit::ET_EMOTION_AVATAR6;
		return true;
#endif // CRAYONPOP_SECOND_EMOTION

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
	case CX2Unit::ET_EMOTION_AVATAR7:
		wstrName = L"Emotion_BbaBbaBba";
		eId = CX2Unit::ET_EMOTION_AVATAR7;
		return true;
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨

	default:
		break;
	}

	return false;
}

void CX2ChatBox::SendEmotionId( wstring wstrEmotionName)
{
#ifdef PLAY_EMOTION_BY_USER_SELECT
	if( m_fSendEmotionIdDelay > 0.f )
		return;

	m_fSendEmotionIdDelay = 2.f;
#endif // PLAY_EMOTION_BY_USER_SELECT

	if(wstrEmotionName.empty() == false)
		Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CPT_TOTAL, CX2Room::RCT_ALL, -1, wstrEmotionName );
}

wstring CX2ChatBox::GetEmotionSound(CX2Unit::EMOTION_TYPE eId) const
{
	switch(eId)
	{	
	case CX2Unit::ET_ANGRY:
		return L"Emotion_Angry.ogg";
	case CX2Unit::ET_HELLO:
		return L"Emotion_Hello.ogg";
	case CX2Unit::ET_NO:
		return L"Emotion_No.ogg";
	case CX2Unit::ET_SAD:
		return L"Emotion_Sad.ogg";
	case CX2Unit::ET_SMILE:
		return L"Emotion_Smile.ogg";
	case CX2Unit::ET_EMOTION_AVATAR2:
		return L"Newyear.ogg";
	case CX2Unit::ET_EMOTION_AVATAR3:
		return L"Emotion_APinkDance.ogg";
	case CX2Unit::ET_EMOTION_AVATAR5:
		return L"DancingQueen_Music.ogg";
#ifdef CRAYONPOP_SECOND_EMOTION
	case CX2Unit::ET_EMOTION_AVATAR6:
		return L"Emotion_Bbabbabba.ogg";
#endif // CRAYONPOP_SECOND_EMOTION
#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
	case CX2Unit::ET_EMOTION_AVATAR7:
		return L"Emotion_Crayonpop_NoMotion.ogg";
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨

	default:
		return L"";
	}

	return L"";
}



bool CX2ChatBox::PlayEmotionSound( CX2Unit::UNIT_TYPE eUnitType, CX2Unit::EMOTION_TYPE eEmotionID, const D3DXVECTOR3& vPos, bool b3DSound )
{
	wstring wstrEmotionSoundSuffix = GetEmotionSound(eEmotionID);
	if( true == wstrEmotionSoundSuffix.empty() )
		return false;

#ifdef REMOVE_ARA_VOICE_TEMP
	if( eUnitType == CX2Unit::UT_ARA )
	{
		if( eEmotionID == CX2Unit::ET_SAD || eEmotionID == CX2Unit::ET_HELLO || eEmotionID == CX2Unit::ET_NO 
			|| eEmotionID == CX2Unit::ET_ANGRY || eEmotionID == CX2Unit::ET_SMILE )
		{
			return false;
		}
	}
#endif //REMOVE_ARA_VOICE_TEMP 

	CX2Unit::EMOTION_TYPE eEmotion = CX2Unit::ET_NONE;

	bool bAvatarEmotion = false;
	
	if ( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		bAvatarEmotion = g_pData->GetMyUser()->GetSelectUnit()->GetAvatarEmotionID(eEmotion);
	}

	wstring wstrEmotionSound = L"";
	switch( eUnitType )
	{
	case CX2Unit::UT_ELSWORD:
		wstrEmotionSound = L"Elsword_";
		break;
	case CX2Unit::UT_ARME:
		wstrEmotionSound = L"Aisha_";
		break;
	case CX2Unit::UT_LIRE:
		wstrEmotionSound = L"Lena_";
		break;
	case CX2Unit::UT_RAVEN:
		wstrEmotionSound = L"Raven_";
		break;
	case CX2Unit::UT_EVE:
		wstrEmotionSound = L"Eve_";
		break;
//{{ kimhc // 2010.11.17 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG:
		wstrEmotionSound = L"Chung_";
		break;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.17 //  2010-12-23 New Character CHUNG
#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UT_ARA:
		wstrEmotionSound = L"Ara_";
		break;
#endif
#ifdef NEW_CHARACTER_EL
	case CX2Unit::UT_ELESIS:
		wstrEmotionSound = L"Elesis_";
		break;
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER
	case CX2Unit::UT_ADD:
		wstrEmotionSound = L"Add_";
		break;
#endif //SERV_9TH_NEW_CHARACTER

	default:
		break;
	}

	if( CX2Unit::IsAvatarEmotion( eEmotionID ) &&
		(bAvatarEmotion == false || eEmotionID != eEmotion ) )
		return false;

	if( eEmotionID == CX2Unit::ET_EMOTION_AVATAR1 || eEmotionID == CX2Unit::ET_EMOTION_AVATAR3 || eEmotionID == CX2Unit::ET_EMOTION_AVATAR5 
#ifdef CRAYONPOP_SECOND_EMOTION
		|| eEmotionID == CX2Unit::ET_EMOTION_AVATAR6
#endif // CRAYONPOP_SECOND_EMOTION

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
		|| eEmotionID == CX2Unit::ET_EMOTION_AVATAR7
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
		)
		wstrEmotionSound = wstrEmotionSoundSuffix;
	else
		wstrEmotionSound += wstrEmotionSoundSuffix;

	const float MAGIC_NUMBER = 500.f;
	const float fMaxDist = g_pKTDXApp->GetDGManager()->GetCamera().GetCameraDistance() + MAGIC_NUMBER;
	CKTDXDeviceSound* pDeviceSound = g_pKTDXApp->GetDeviceManager()->OpenSound( wstrEmotionSound.c_str(), 10, true, fMaxDist );
	if( NULL == pDeviceSound )
		return false;

	m_vecDeviceSound.push_back( pDeviceSound );
	

	switch( g_pMain->GetNowStateID() )
	{
	// NOTE!! 거래 광장에서는 일단 2D사운드로.
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_PVP_ROOM:
#ifdef ADDED_RELATIONSHIP_SYSTEM// 웨딩도 일단 2D사운드로..
	case CX2Main::XS_WEDDING_GAME: 
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			pDeviceSound->Play( false, false );
		} break;
	default:
		{
			pDeviceSound->Set3DPosition( vPos );
			pDeviceSound->Play( false );
		} break;
	}

	return true;
}






#endif

//{{ 허상형 : [2009/7/14] // 나소드 메가폰 관련 함수
bool CX2ChatBox::ShowNasodMessageDlg( bool bEnable )	//	나소드 메가폰 메시지 입력 Dialog 생성
{
	if(bEnable == true)
	{
#ifdef NASOD_SCOPE_ID_SAVE
		if( -1 == m_iLastUsedMegaphoneTID && NULL != m_pSelectedItemSlot )
		{
			m_iLastUsedMegaphoneTID = m_pSelectedItemSlot->GetItemTID();
		}
#endif //NASOD_SCOPE_ID_SAVE

		m_pDLGNasodScopeMessage = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Use_Nasod_Scope.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGNasodScopeMessage );
		
		return true;
	}
	else
	{
		if(m_pDLGNasodScopeMessage != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGNasodScopeMessage, NULL, false );
			m_pDLGNasodScopeMessage = NULL;
			//	선택된 아이템 슬롯 제거
			SetItemSlot( NULL );
			// oasis907 : 김상윤 [2010.3.29] // 
			m_iLastUsedMegaphoneTID = -1;
		}


		if( m_pDLGNasodScopeUserMenu != NULL )
		{
			m_pDLGNasodScopeUserMenu->SetShowEnable(false, false);
		}

		return true;
	}
	
	return false;
}

//	핸들러
bool CX2ChatBox::Handler_EGS_USE_MEGAPHONE_REQ()
{
	if( m_pDLGNasodScopeMessage != NULL)
	{
		CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGNasodScopeMessage->GetControl( L"IMEEditBoxMessage" );
		if(pIMEEditBox != NULL)
		{
			if(m_pSelectedItemSlot != NULL || m_iLastUsedMegaphoneTID != -1)
			{
				KEGS_USE_MEGAPHONE_REQ kPacket;

				if(m_iLastUsedMegaphoneTID != -1)
				{
					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( m_iLastUsedMegaphoneTID );
					if(pItem != NULL)
					{
						kPacket.m_iItemUID = pItem->GetUID();
					}
				}
				else
				{
					kPacket.m_iItemUID = m_pSelectedItemSlot->GetItemUID();
				}
				kPacket.m_wstrMessage = pIMEEditBox->GetText();

				// 크기가 91byte일 경우(multi - byte의 경우 마지막 데이터 삭제)
				std::string strMessage = KncUtil::toNarrowString( kPacket.m_wstrMessage );

				if( strMessage.size() > 90 )
				{
					kPacket.m_wstrMessage = kPacket.m_wstrMessage.substr(0, kPacket.m_wstrMessage.size()-1);
				}

				g_pData->GetServerProtocol()->SendPacket( EGS_USE_MEGAPHONE_REQ, kPacket );
				g_pMain->AddServerPacket( EGS_USE_MEGAPHONE_ACK );

				SetItemSlot( NULL );

				// oasis907 : 김상윤 [2010.3.29] // 
				m_iLastUsedMegaphoneTID = -1;
	
				return true;
			}
		}
	}
	
	return false;
}

bool CX2ChatBox::Handler_EGS_USE_MEGAPHONE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USE_MEGAPHONE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if(g_pMain->DeleteServerPacket( EGS_USE_MEGAPHONE_ACK ))
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// npc join상태이거나 대전로비/대전방에서 메가폰사용후 채팅창 강제로 open해준다.			
			if( (g_pTFieldGame != NULL && g_pTFieldGame->GetJoinNpc() == true) ||
				g_pMain->GetNowStateID() == CX2Main::XS_PVP_LOBBY || 
				g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM )
			{				
				OpenChatWindow();
				UnHideChatWindowFrame();
				UnHideChatEditBox();

				m_bOpenChatWindowButton = true;	
			}

			//	다이얼로그 닫기
			if( ShowNasodMessageDlg( false ) )
			{
				CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
				//	아이템 업데이트
				kInventory.UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );


				//{{ oasis907 : 김상윤 [2010.3.29] // 
				if( g_pData->GetUIManager()->GetUIPersonalShop() != NULL)
				{
					g_pData->GetUIManager()->GetUIPersonalShop()->UpdateMegaphoneButton();
				}
				//}}

				return true;
			}
		}
	}

	return false;
}

bool CX2ChatBox::Handler_EGS_MEGAPHONE_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_MEGAPHONE_MESSAGE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_BATTLE_FIELD:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME: 
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			//	튜토리얼 중이면 메세지 출력 안함
			if( true == g_pMain->GetIsPlayingTutorial() )
			{
				return true;
			}
			
			m_wstrSenderName = kEvent.m_wstrSenderNickName;
			AddChatLog( GET_REPLACED_STRING( ( STR_ID_3739, "LLL", kEvent.m_wstrSenderNickName, g_pInstanceData->GetChannelServerName( kEvent.m_iSenderChannelID ), kEvent.m_wstrMessage) ),
				KEGS_CHAT_REQ::CPT_MEGAPHONE, D3DXCOLOR(1,1,1,1), L"", true, false );

#ifdef CHAT_LOG_TO_TXT
			if ( g_pChatBox != NULL )
			{
				wstring chatContent = kEvent.m_wstrSenderNickName;
				chatContent += L" : ";
				chatContent += kEvent.m_wstrMessage;
				g_pChatBox->AddChatLogToFile( chatContent.c_str() );
			}
#endif // CHAT_LOG_TO_TXT

			//	채팅 로그에 추가 한 후 아이디 정보를 지운다.
			m_wstrSenderName = L"";
			return true;

		} break;
	}

	return true;

}
//}} 허상형 : [2009/7/14] // 나소드 메가폰 관련 함수

//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
bool CX2ChatBox::Handler_EGS_ANTI_ADDICTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ANTI_ADDICTION_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	switch(kEvent.m_iType)
	{
	case KEGS_ANTI_ADDICTION_NOT::NT_CHAT:
		{
			wstringstream wstrmTotalPlayTime;

			int hours = kEvent.m_iPlayTime / 3600/*sec*/;
			if(hours)
			{
				wstrmTotalPlayTime << GET_REPLACED_STRING( ( STR_ID_310, "i", hours ) );
			}
			int mins = (kEvent.m_iPlayTime % 3600) / 60/*sec*/;
			if(mins)
			{
				wstrmTotalPlayTime << GET_REPLACED_STRING( ( STR_ID_4929, "i", mins ) );
			}

			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_VILLAGE_MAP:
			case CX2Main::XS_DUNGEON_GAME:
			case CX2Main::XS_SQUARE_GAME:
			case CX2Main::XS_PVP_ROOM:
			case CX2Main::XS_PVP_GAME:
			case CX2Main::XS_PVP_LOBBY:
				{
					//	튜토리얼 중이면 메세지 출력 안함
					if( true == g_pMain->GetIsPlayingTutorial() )
					{
						return true;
					}

					AddChatLog( GET_REPLACED_STRING( ( STR_ID_4941, "L", wstrmTotalPlayTime.str() ) ), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

					return true;
				}
				break;

			}
		}
		break;
	case KEGS_ANTI_ADDICTION_NOT::NT_CENTER:
		{
			g_pMain->GetNoticeManager()->AddNotice( KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_ALL_SERVERS, GET_STRING( STR_ID_4942 ) );
		}
		break;
	default:
		break;
	}

	return true;
}
#endif SERV_ANTI_ADDICTION_SYSTEM
//}} 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템

bool CX2ChatBox::Handler_EGS_CHAT_OPTION_INFO_WRITE_REQ(byte cIndex, bool bVal)
{
	KEGS_CHAT_OPTION_INFO_WRITE_REQ kPacket;
	kPacket.m_cIndex = cIndex;
	kPacket.m_bValue = bVal;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHAT_OPTION_INFO_WRITE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHAT_OPTION_INFO_WRITE_ACK );

	return true;
}
bool CX2ChatBox::Handler_EGS_CHAT_OPTION_INFO_WRITE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHAT_OPTION_INFO_WRITE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHAT_OPTION_INFO_WRITE_ACK ) == true )
	{		
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////
CX2ChatBox::AntiChatSpam::AntiChatSpam()
{
	m_iMaxSpamWarningCount = 3;
	m_iNowSpamWarningCount = 0;
	m_fSpamBlockingTimeLeft = 0.f;

	m_fMaxSpamBlockingTime1 = 30.f;
	m_fMaxSpamBlockingTime2 = 30.f;
	m_fMaxSpamBlockingTime3 = 60.f * 5.f;

	m_bSpamMessage = false;
}

void CX2ChatBox::AntiChatSpam::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	//if( m_iNowSpamWarningCount >= m_iMaxSpamWarningCount )
	//{
	//	m_iNowSpamWarningCount = 0;
	//	m_fSpamBlockingTimeLeft = m_fMaxSpamBlockingTime;
	//}
	if(m_bSpamMessage == true)
	{
		switch(m_iNowSpamWarningCount)
		{
		case 0:
			m_iNowSpamWarningCount = 0;
			m_fSpamBlockingTimeLeft = 0.f;
			break;
		case 1:
			m_fSpamBlockingTimeLeft = m_fMaxSpamBlockingTime1;
			break;
		case 2:
			m_fSpamBlockingTimeLeft = m_fMaxSpamBlockingTime2;
			break;
		case 3:
			m_iNowSpamWarningCount = 0;
			m_fSpamBlockingTimeLeft = m_fMaxSpamBlockingTime3;
			break;
		default:
			m_iNowSpamWarningCount = 0;
			m_fSpamBlockingTimeLeft = m_fMaxSpamBlockingTime3;
			break;
		}
	}

	m_bSpamMessage = false;

	if( m_fSpamBlockingTimeLeft > 0.f )
	{
		m_fSpamBlockingTimeLeft -= fElapsedTime;
		if( m_fSpamBlockingTimeLeft < 0.f )
		{
			m_fSpamBlockingTimeLeft = 0.f;			

			// 스팸 기록 초기화
			if(m_iNowSpamWarningCount >= m_iMaxSpamWarningCount)
			{
				m_iNowSpamWarningCount = 0;
				m_qChatLog.clear();
				m_mapChatLogCount.clear();
			}			
		}
	}
	else
	{
		// 일정시간이 지나면 spam candidate chatlog에서 문장을 삭제
		const float MAGIC_VANISHING_TIME = 60.f;				

		if( m_qChatLog.size() > 0 )
		{
			ChatLog& chatLog = m_qChatLog.front();
			if( g_pKTDXApp->GetTime() - chatLog.m_fTime > MAGIC_VANISHING_TIME )
			{
				std::map<wstring, int>::iterator it = m_mapChatLogCount.find( chatLog.m_wstrMsg );
				if( it != m_mapChatLogCount.end() )
				{
					m_mapChatLogCount.erase( it );
				}
				m_qChatLog.pop_front();
			}

			if( m_qChatLog.size() == 0 && m_mapChatLogCount.size() != 0 )
			{
				m_mapChatLogCount.clear();
			}
		}
	}
}

void CX2ChatBox::AntiChatSpam::ClearLastChatLog()
{
	if( m_qChatLog.size() > 0 )
	{
		ChatLog& chatLog = m_qChatLog.back();
		std::map<wstring, int>::iterator it;
		it = m_mapChatLogCount.find( chatLog.m_wstrMsg );
		if( it != m_mapChatLogCount.end() )
		{
			it->second -= 1;
			if( it->second <= 0 )
			{
				m_mapChatLogCount.erase( it );
			}
		}
		m_qChatLog.pop_back();
	}
}

bool CX2ChatBox::AntiChatSpam::IsThisMessageSpam( wstring& wstrMsg )
{
#ifdef CLIENT_COUNTRY_US
	// 개발자는 채팅금지 안먹는다.
	if( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
		return false;
#endif //CLIENT_COUNTRY_US

	m_bSpamMessage = false;

	if( m_fSpamBlockingTimeLeft > 0.f )
		return true;				

	const int MAGIC_MAX_SAME_STRING_COUNT = 4;
	//const int MAGIC_LONG_STRING_LENGTH_BOUND = 8;
	const float MAGIC_STRING_INPUT_TIME_LIMIT = 1.f;
	const int MAGIC_MAX_STRING_COUNT_IN_TIME_LIMIT = 3;

	const UINT MAGIC_MAX_CHAT_LOG_QUEUE_SIZE = 12;



	std::map<wstring, int>::iterator it;
	while( m_qChatLog.size() > MAGIC_MAX_CHAT_LOG_QUEUE_SIZE )
	{
		ChatLog& chatLog = m_qChatLog[0];
		it = m_mapChatLogCount.find( chatLog.m_wstrMsg );
		if( it != m_mapChatLogCount.end() )
		{
			it->second -= 1;

			if( it->second <= 0 )
			{
				m_mapChatLogCount.erase( it );
			}
		}

		m_qChatLog.pop_front();
	}

	ChatLog newChatLog;
	newChatLog.m_fTime = g_pKTDXApp->GetTime();
	newChatLog.m_wstrMsg = wstrMsg;
	m_qChatLog.push_back( newChatLog );

	it = m_mapChatLogCount.find( newChatLog.m_wstrMsg );
	if( it != m_mapChatLogCount.end() )
	{
		it->second += 1;
	}
	else
	{
		m_mapChatLogCount[ newChatLog.m_wstrMsg ] = 1;
	}

	if( m_qChatLog.size() > MAGIC_MAX_STRING_COUNT_IN_TIME_LIMIT ) 
	{
		ChatLog& chatLog0 = m_qChatLog[ m_qChatLog.size() - MAGIC_MAX_STRING_COUNT_IN_TIME_LIMIT ];
		ChatLog& chatLog1 = m_qChatLog[ m_qChatLog.size() - 1 ];

		if( chatLog1.m_fTime - chatLog0.m_fTime < MAGIC_STRING_INPUT_TIME_LIMIT )
		{
			// 도배다!!
			m_iNowSpamWarningCount++;
			m_bSpamMessage = true;
			return true;
		}
	}

	if( m_qChatLog.size() >= MAGIC_MAX_SAME_STRING_COUNT )
	{
		for( it=m_mapChatLogCount.begin(); it!=m_mapChatLogCount.end(); it++ )
		{
			if( it->second >= MAGIC_MAX_SAME_STRING_COUNT )
			{
				// 도배다!!
				m_iNowSpamWarningCount++;
				ClearLastChatLog();
				m_bSpamMessage = true;
				return true;
			}
		}
	}

	return false;
}

CKTDGUIListBox* CX2ChatBox::GetCurrentChatbox()
{
	CKTDGUIListBox* pChatBox = NULL;

	switch( GetChatWindowMode() )
	{
	case CWM_TOTAL:
		{
			pChatBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_All" );
		} break;
	case CWM_PARTY:
		{
			pChatBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Party" );
		} break;

	case CWM_GUILD:
		{
			pChatBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Guild" );
		} break;
	case CWM_PERSONAL:
		{
			pChatBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Personal" );
		} break;
	case CWM_MEGAPHONE:
		{
			pChatBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Megaphone" );
		} break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CWM_RELATION_COUPLE :
	case CWM_RELATION_MARRIED :
		{
			pChatBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Relation" );
		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM

	}

	return pChatBox;
}

//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
void CX2ChatBox::ChangeIMEEditBox( CHAT_WINDOW_MODE eIMEEditWindowMode, bool bUseTabKey /* = false */ )
{
	if ( eIMEEditWindowMode == CWM_MEGAPHONE )	// 메가폰인 경우 실행하지 않음
		return;

	CHAT_WINDOW_MODE	eBeforeMode = GetIMEEditWindowMode();

	SetIMEEditWindowMode( eIMEEditWindowMode );

	CKTDGUIStatic*		pStatic_ChatMode	= static_cast< CKTDGUIStatic* >( m_pDLGChatWindowEditBox->GetControl( L"Chat_Mode" ) );
	CKTDGUIIMEEditBox*	pIMEEditBox			= static_cast< CKTDGUIIMEEditBox* >( m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" ) );

#ifdef ADDED_RELATIONSHIP_SYSTEM

	for ( int i = 0; i < _CONST_CHATBOX_::g_iMaxChatMode; i++ )
		pStatic_ChatMode->GetPicture( i )->SetShow( false );

#else // ADDED_RELATIONSHIP_SYSTEM
	for ( int i = 0; i < 4; i++ )
		pStatic_ChatMode->GetPicture( i )->SetShow( false );
#endif // ADDED_RELATIONSHIP_SYSTEM
	
	
	switch ( GetIMEEditWindowMode() )
	{
	case CWM_TOTAL:
		pStatic_ChatMode->GetPicture( CWM_TOTAL )->SetShow( true );
		break;

	case CWM_PARTY:
		pStatic_ChatMode->GetPicture( CWM_PARTY )->SetShow( true );
		break;

	case CWM_GUILD:
		pStatic_ChatMode->GetPicture( CWM_GUILD )->SetShow( true );
		break;

	case CWM_PERSONAL:
		pStatic_ChatMode->GetPicture( CWM_PERSONAL )->SetShow( true );
		break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CWM_RELATION_COUPLE :	
		pStatic_ChatMode->GetPicture( CWM_RELATION_COUPLE )->SetShow( true );
		break;

	case CWM_RELATION_MARRIED :	
		pStatic_ChatMode->GetPicture( CWM_RELATION_MARRIED )->SetShow( true );
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM

	default:
		break;
	}

	if ( pIMEEditBox == NULL )
		return;

//{{ kimhc // 2010.3.15 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
	// ClearText가 제대로 되지 않는 현상이 있어서 임시로 수정함 // 정확한 원인을 찾아봐야 함
	// 귓말로 넘어갈때만 지워지도록 수정
	
	if ( bUseTabKey == false )		// 탭에 의한 변경이 아니면
	{
		GetChatMessage();
	}

	if ( bUseTabKey == true && (eBeforeMode == CWM_PERSONAL ||				// 변경전의 입력모드가 귓말이거나
		 GetIMEEditWindowMode() == CWM_PERSONAL) )	// 변경후의 입력모드가 귓말이면
	{
		GetChatMessage();
		pIMEEditBox->RequestFocus();
	}

	if ( GetIMEEditWindowMode() == CWM_PERSONAL )
	{
#ifdef GLOBAL_CHAT_FUNCTION
		wstring wstrlastWhiperUser = GET_STRING(STR_ID_14331);
		wstrlastWhiperUser += L" ";
#else GLOBAL_CHAT_FUNCTION
		wstring wstrlastWhiperUser = L"/w ";
#endif GLOBAL_CHAT_FUNCTION

		if(m_wstrLastWhiperNickName.empty() == false)
		{
			wstrlastWhiperUser += m_wstrLastWhiperNickName;
			wstrlastWhiperUser += L" ";
		}					

		pIMEEditBox->SetText( wstrlastWhiperUser.c_str() );

	}

	

#else	CHAT_WINDOW_IMPROV
	if ( GetIMEEditWindowMode() != CWM_PERSONAL )
	{
		pIMEEditBox->ClearText();
	}
	else
	{
#ifdef GLOBAL_CHAT_FUNCTION
		wstring wstrlastWhiperUser = GET_STRING(STR_ID_14331);
		wstrlastWhiperUser += L" ";
#else GLOBAL_CHAT_FUNCTION
		wstring wstrlastWhiperUser = L"/w ";
#endif GLOBAL_CHAT_FUNCTION

		if(m_wstrLastWhiperNickName.empty() == false)
		{
			wstrlastWhiperUser += m_wstrLastWhiperNickName;
			wstrlastWhiperUser += L" ";
		}					

		pIMEEditBox->SetText( wstrlastWhiperUser.c_str() );
	}
#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.15 //	채팅창 개편

}
void CX2ChatBox::ChangeChatWindow( CHAT_WINDOW_MODE eMode )
{
	SetChatWindowMode( eMode );

	if( NULL == m_pDLGChatWindow )
		return;


	CKTDGUIListBox* pListBox_Square = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_All" );
	CKTDGUIListBox* pListBox_Party = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Party" );
	CKTDGUIListBox* pListBox_Guild = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Guild" );
	CKTDGUIListBox* pListBox_Personal = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Personal" );
	//{{ 허상형 : [2009/7/17] //	나소드 메가폰 리스트박스 컨트롤
	CKTDGUIListBox* pListBox_Megaphone = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( L"ListBox_Megaphone" );
	//}} 허상형 : [2009/7/17] //	나소드 메가폰 리스트박스 컨트롤
#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIListBox* pListBox_Relation = static_cast <CKTDGUIListBox*> ( m_pDLGChatWindow->GetControl( L"ListBox_Relation" ) );
#endif // ADDED_RELATIONSHIP_SYSTEM

	CKTDGUIRadioButton* pRadio_All		= (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonAll" );
	CKTDGUIRadioButton* pRadio_Party	= (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonParty" );
	CKTDGUIRadioButton* pRadio_Guild	= (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonGuild" );
	CKTDGUIRadioButton* pRadio_Personal	= (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonPersonal" );
#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIRadioButton* pRadio_RelationCouple	= (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonRelationCouple" );
	CKTDGUIRadioButton* pRadio_RelationMarried	= (CKTDGUIRadioButton*) m_pDLGChatWindow->GetControl( L"ButtonRelationMarried" );
#endif // ADDED_RELATIONSHIP_SYSTEM
	//{{ 허상형 : [2009/7/23] //	탭 변경 시 클릭 지워지게
	if( pListBox_Square != NULL)	pListBox_Square->ClearSelect();
	if( pListBox_Party != NULL)		pListBox_Party->ClearSelect();
	if( pListBox_Guild != NULL)		pListBox_Guild->ClearSelect();
	if( pListBox_Personal != NULL)	pListBox_Personal->ClearSelect();
	if( pListBox_Megaphone != NULL)	pListBox_Megaphone->ClearSelect();
#ifdef ADDED_RELATIONSHIP_SYSTEM
	if( pListBox_Relation != NULL)	pListBox_Relation->ClearSelect();
#endif // ADDED_RELATIONSHIP_SYSTEM

	//}} 허상형 : [2009/7/23] //	탭 변경 시 클릭 지워지게


	switch( GetChatWindowMode() )
	{
	case CWM_TOTAL:
		{
			pListBox_Square->SetEnable( true );
			pListBox_Square->SetShow( true );
			pListBox_Party->SetEnable( false );
			pListBox_Party->SetShow( false );
			pListBox_Guild->SetEnable( false );
			pListBox_Guild->SetShow( false );
			pListBox_Personal->SetShowEnable( false, false );
			//{{ 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부
			pListBox_Megaphone->SetShowEnable( false, false );
			//}} 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부


			pRadio_All->SetChecked( true );
			pRadio_Party->SetChecked( false );
			pRadio_Guild->SetChecked( false );
			pRadio_Personal->SetChecked( false );
#ifdef ADDED_RELATIONSHIP_SYSTEM
			pListBox_Relation->SetShowEnable( false, false );
			pRadio_RelationMarried->SetChecked(false);
			pRadio_RelationCouple->SetChecked(false);
#endif // ADDED_RELATIONSHIP_SYSTEM

		} break;

	case CWM_PARTY:
		{
			pListBox_Square->SetEnable( false );
			pListBox_Square->SetShow( false );
			pListBox_Party->SetEnable( true );
			pListBox_Party->SetShow( true );
			pListBox_Guild->SetEnable( false );
			pListBox_Guild->SetShow( false );
			pListBox_Personal->SetShowEnable( false, false );
			//{{ 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부
			pListBox_Megaphone->SetShowEnable( false, false );
			//}} 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부

			pRadio_All->SetChecked( false );
			pRadio_Party->SetChecked( true );
			pRadio_Guild->SetChecked( false );
			pRadio_Personal->SetChecked( false );
#ifdef ADDED_RELATIONSHIP_SYSTEM
			pListBox_Relation->SetShowEnable( false, false );
			pRadio_RelationCouple->SetChecked(false);
			pRadio_RelationMarried->SetChecked(false);
#endif // ADDED_RELATIONSHIP_SYSTEM

		} break;

	case CWM_GUILD:
		{
			//{{ 허상형 : [2009/9/16] //	길드 버튼 활성화

#ifdef	GUILD_MANAGEMENT
			pListBox_Square->SetEnable( false );
			pListBox_Square->SetShow( false );
			pListBox_Party->SetEnable( false );
			pListBox_Party->SetShow( false );
			pListBox_Guild->SetEnable( true );
			pListBox_Guild->SetShow( true );
			pListBox_Personal->SetShowEnable( false, false );
			//{{ 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부
			pListBox_Megaphone->SetShowEnable( false, false );
			//}} 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부

			pRadio_All->SetChecked( false );
			pRadio_Party->SetChecked( false );
			pRadio_Guild->SetChecked( true );
			pRadio_Personal->SetChecked( false );
#endif	//	GUILD_MANAGEMENT
			//}} 허상형 : [2009/9/16] //	길드 버튼 활성화
#ifdef ADDED_RELATIONSHIP_SYSTEM
			pListBox_Relation->SetShowEnable( false, false );
			pRadio_RelationCouple->SetChecked(false);
			pRadio_RelationMarried->SetChecked(false);
#endif // ADDED_RELATIONSHIP_SYSTEM

		} break;

	case CWM_PERSONAL:
		{

			pListBox_Square->SetEnable( false );
			pListBox_Square->SetShow( false );
			pListBox_Party->SetEnable( false );
			pListBox_Party->SetShow( false );
			pListBox_Guild->SetEnable( false );
			pListBox_Guild->SetShow( false );
			pListBox_Personal->SetShowEnable( true, true );
			//{{ 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부
			pListBox_Megaphone->SetShowEnable( false, false );
			//}} 허상형 : [2009/7/17] //	리스트박스 버튼 활성화 여부

			pRadio_All->SetChecked( false );
			pRadio_Party->SetChecked( false );
			pRadio_Guild->SetChecked( false );
			pRadio_Personal->SetChecked( true );

#ifdef ADDED_RELATIONSHIP_SYSTEM
			pListBox_Relation->SetShowEnable( false, false );
			pRadio_RelationCouple->SetChecked(false);
			pRadio_RelationMarried->SetChecked(false);
#endif // ADDED_RELATIONSHIP_SYSTEM

		} break;

		//{{ 허상형 : [2009/7/17] //	메가폰 탭 변경 설정
	case CWM_MEGAPHONE:
		{

			pListBox_Square->SetEnable( false );
			pListBox_Square->SetShow( false );
			pListBox_Party->SetEnable( false );
			pListBox_Party->SetShow( false );
			pListBox_Guild->SetEnable( false );
			pListBox_Guild->SetShow( false );
			pListBox_Personal->SetShowEnable( false, false );
			pListBox_Megaphone->SetShowEnable( true, true );


			pRadio_All->SetChecked( false );
			pRadio_Party->SetChecked( false );
			pRadio_Guild->SetChecked( false );
			pRadio_Personal->SetChecked( false );

#ifdef ADDED_RELATIONSHIP_SYSTEM
			pListBox_Relation->SetShowEnable( false, false );
			pRadio_RelationMarried->SetChecked(false);
			pRadio_RelationCouple->SetChecked(false);
#endif // ADDED_RELATIONSHIP_SYSTEM

		} break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CWM_RELATION_COUPLE :
	case CWM_RELATION_MARRIED :
		{

			pListBox_Square->SetEnable( false );
			pListBox_Square->SetShow( false );
			pListBox_Party->SetEnable( false );
			pListBox_Party->SetShow( false );
			pListBox_Guild->SetEnable( false );
			pListBox_Guild->SetShow( false );
			pListBox_Personal->SetShowEnable( false, false );
			pListBox_Megaphone->SetShowEnable( false, false );


			pRadio_All->SetChecked( false );
			pRadio_Party->SetChecked( false );
			pRadio_Guild->SetChecked( false );
			pRadio_Personal->SetChecked( false );

			if ( NULL != g_pData->GetRelationshipManager() && 
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() ) 
			{
				switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
				{
				case SEnum::RT_COUPLE :
					pRadio_RelationCouple->SetChecked(true);
					pRadio_RelationMarried->SetChecked(false);
					break;

				case SEnum::RT_MARRIED :				
					pRadio_RelationCouple->SetChecked(false);
					pRadio_RelationMarried->SetChecked(true);
					break;
				}
			}


			pListBox_Relation->SetShowEnable( true, true );

		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM

		//}} 허상형 : [2009/7/17] //
	}

	const float MAGIC_CHAT_SHOW_TIME = 50.0f;

	//{{ 허상형 : [2009/7/24] //	탭 이동시 메시지 보이게 설정
	pListBox_Square->SetShowAllItems( true );
	pListBox_Square->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
	pListBox_Party->SetShowAllItems( true );
	pListBox_Party->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
	pListBox_Guild->SetShowAllItems( true );
	pListBox_Guild->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
	pListBox_Personal->SetShowAllItems( true );
	pListBox_Personal->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );

#ifdef ADDED_RELATIONSHIP_SYSTEM
	pListBox_Relation->SetShowAllItems( true );
	pListBox_Relation->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
#endif // ADDED_RELATIONSHIP_SYSTEM


	//{{ 허상형 : [2009/7/24] //
	pListBox_Megaphone->SetShowAllItems( true );
	pListBox_Megaphone->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
	//}} 허상형 : [2009/7/24] //
	if( true == m_pDLGTabMenu->GetShow() )
		m_pDLGTabMenu->SetShow(false);
}

void CX2ChatBox::ProcessChangeIMEEditWindowByTabKey()
{
	bool bIsShiftPushing = false;

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_LSHIFT ) == TRUE ||
		 g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_RSHIFT ) == TRUE )
		 bIsShiftPushing = true;

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_TAB ) == TRUE )
	{
		g_pKTDXApp->GetDIManager()->Getkeyboard()->SetLock( DIK_TAB, TRUE );

		CHAT_WINDOW_MODE eIMEEditWindowMode = GetIMEEditWindowMode();

		if ( bIsShiftPushing == false )
		{
			switch ( eIMEEditWindowMode )
			{
			case CWM_TOTAL:
				eIMEEditWindowMode = CWM_PARTY;
				break;

			case CWM_PARTY:
				eIMEEditWindowMode = CWM_GUILD;
				break;

			case CWM_GUILD:
				eIMEEditWindowMode = CWM_PERSONAL;
				break;

			case CWM_PERSONAL:
#ifdef ADDED_RELATIONSHIP_SYSTEM				
				if ( NULL != g_pData->GetRelationshipManager() && 
					NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() ) 
				{
					switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
					{
					case SEnum::RT_SOLO :
						eIMEEditWindowMode = CWM_TOTAL;
						break;

					case SEnum::RT_COUPLE :
						eIMEEditWindowMode = CWM_RELATION_COUPLE;
						break;

					case SEnum::RT_MARRIED :				
						eIMEEditWindowMode = CWM_RELATION_MARRIED;
						break;
					}
				}
				break;

			case CWM_RELATION_COUPLE:
			case CWM_RELATION_MARRIED:
				eIMEEditWindowMode = CWM_TOTAL;

#else // ADDED_RELATIONSHIP_SYSTEM
				eIMEEditWindowMode = CWM_TOTAL;
				break;
#endif // ADDED_RELATIONSHIP_SYSTEM				

			default:
				eIMEEditWindowMode = CWM_TOTAL;
				break;
			}
		}
		else
		{
			switch ( eIMEEditWindowMode )
			{

			case CWM_TOTAL:
				eIMEEditWindowMode = CWM_PERSONAL;
				break;


#ifdef ADDED_RELATIONSHIP_SYSTEM				
			case CWM_RELATION_COUPLE:
			case CWM_RELATION_MARRIED:
				eIMEEditWindowMode = CWM_TOTAL;
				break;
#endif // ADDED_RELATIONSHIP_SYSTEM				


			case CWM_PARTY:
#ifdef ADDED_RELATIONSHIP_SYSTEM
				if ( NULL != g_pData->GetRelationshipManager() && 
					NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() ) 
				{
					switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
					{
					case SEnum::RT_SOLO :
						eIMEEditWindowMode = CWM_TOTAL;
						break;

					case SEnum::RT_COUPLE :
						eIMEEditWindowMode = CWM_RELATION_COUPLE;
						break;

					case SEnum::RT_MARRIED :				
						eIMEEditWindowMode = CWM_RELATION_MARRIED;
						break;
					}
				}	
#else // ADDED_RELATIONSHIP_SYSTEM
				eIMEEditWindowMode = CWM_TOTAL;
#endif // ADDED_RELATIONSHIP_SYSTEM
				break;

			case CWM_GUILD:
				eIMEEditWindowMode = CWM_PARTY;
				break;

			case CWM_PERSONAL:
				eIMEEditWindowMode = CWM_GUILD;
				break;

			default:
				eIMEEditWindowMode = CWM_TOTAL;
				break;
			}
		}
		ChangeIMEEditBox( eIMEEditWindowMode, true );
	}
}

void CX2ChatBox::UnHideChatLog( CHAT_WINDOW_MODE eChatWindowMode )
{
	wstring	wstrName;

	switch ( eChatWindowMode )
	{
	case CWM_TOTAL:
		wstrName = L"ListBox_All";
		break;

	case CWM_PARTY:
		wstrName = L"ListBox_Party";
		break;

	case CWM_GUILD:
		wstrName = L"ListBox_Guild";		
		break;

	case CWM_PERSONAL:
		wstrName = L"ListBox_Personal";
		break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CWM_RELATION_COUPLE :
	case CWM_RELATION_MARRIED :
		wstrName = L"ListBox_Relation";
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM

	default:
		return;
		break;
	}

	CKTDGUIListBox* pListBox = static_cast< CKTDGUIListBox* >( m_pDLGChatWindow->GetControl( wstrName.c_str() ) );

	if ( pListBox != NULL )
	{
		const float MAGIC_CHAT_SHOW_TIME = 50.0f;

		pListBox->SetShowAllItems( true );
		pListBox->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
	}
}

void CX2ChatBox::ChangeNativeAndEngKeyTexture()
{
	bool bNative = g_pMain->GetNativeAndEnglisKeyState();

	if ( bNative != m_bPrevNativeLanguage )
	{
		if ( SetNativeAndEngTexture( bNative ) == true )
		{
			m_bPrevNativeLanguage = bNative;	
		}
	}
}

bool CX2ChatBox::SetNativeAndEngTexture( bool bNative )
{
	CKTDGUIStatic* pStatic = static_cast< CKTDGUIStatic* >( m_pDLGChatWindowEditBox->GetControl( L"NativeEnglish" ) );

	if ( pStatic == NULL )
	{
		ASSERT( !L"NativeEnglish is fault" );
		return false;
	}

	if ( bNative == true )
	{
		pStatic->GetPicture(0)->SetShow( true );
		pStatic->GetPicture(1)->SetShow( false );
	}
	else
	{
		pStatic->GetPicture(0)->SetShow( false );
		pStatic->GetPicture(1)->SetShow( true );
	}
	
	return true;
}

wstring CX2ChatBox::GetHistory()
{
	int iSize = m_deqHistoryForMsg.size();

	if ( iSize == 0 )
		return L"";

	if ( GetHistoryIndex() >= iSize )		// 제일 최근 히스토리에서 down을 누른경우에 빈문자열을 보내줌
		return L"";

	if ( GetHistoryIndex() < 0 )
	{
		SetHistoryIndex( 0 );
		return m_deqHistoryForMsg[0];
	}

	return m_deqHistoryForMsg[GetHistoryIndex()];
}

void CX2ChatBox::PushMsgToHistory( const wstring& wstrMsg )
{
	if ( m_deqHistoryForMsg.size() >= 5 )
		m_deqHistoryForMsg.pop_front();

	m_deqHistoryForMsg.push_back( wstrMsg );
}

void CX2ChatBox::ChangeHistoryIndex( bool bUp )
{
	int iSize = m_deqHistoryForMsg.size();

	if ( iSize == 0 )
		return;

	if ( bUp == true )	// 위쪽 화살표가 눌렸으면
	{
		if ( --m_iNowHistoryIndex < 0 )
			SetHistoryIndex( 0 );
	}
	else				// 아래쪽 화살표가 눌렸으면
	{
		if ( ++m_iNowHistoryIndex > iSize )
			SetHistoryIndex( iSize );
	}

	
}

void CX2ChatBox::ProcessInputMsgWithUpKey()
{
	// 이 부분은 매핑을 쓰면 안 됩니다. by 박진웅
//#ifdef REFORM_UI_KEYPAD
//	if ( GET_KEY_STATE( GA_UP ) == TRUE )
//	{
//		SET_KEYLOCK( GA_UP, TRUE );
//#else
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_UP ) == TRUE )
	{
		g_pKTDXApp->GetDIManager()->Getkeyboard()->SetLock( DIK_UP, TRUE );
//#endif

		ChangeHistoryIndex( true );

		CKTDGUIIMEEditBox* pIMEEditBox = static_cast< CKTDGUIIMEEditBox* >( m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" ) );
		wstring wstrMsg = GetHistory();

		SetChatEditText( wstrMsg.c_str() );
	}
}

void CX2ChatBox::ProcessInputMsgWithDownKey()
{
	// 이 부분은 매핑을 쓰면 안 됩니다. by 박진웅
//#ifdef REFORM_UI_KEYPAD
//	if ( GET_KEY_STATE( GA_DOWN ) == TRUE )
//	{
//		SET_KEYLOCK( GA_DOWN, TRUE );
//#else
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_DOWN ) == TRUE )
	{
		g_pKTDXApp->GetDIManager()->Getkeyboard()->SetLock( DIK_DOWN, TRUE );
//#endif

		ChangeHistoryIndex( false );

		CKTDGUIIMEEditBox* pIMEEditBox = static_cast< CKTDGUIIMEEditBox* >( m_pDLGChatWindowEditBox->GetControl( L"IMEEditBoxChatBox" ) );
		wstring wstrMsg = GetHistory();

		SetChatEditText( wstrMsg.c_str() );
	}
}

#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.10 //	채팅창 개편

void CX2ChatBox::ResetChatTabPopup(CX2ChatBox::CHAT_WINDOW_MODE eChatWinMode)
{
	//{{파일 변경 사항 즉시 적용시키기 위해 임시
	//채팅탭 우클릭 메뉴
	SAFE_DELETE_DIALOG(m_pDLGTabMenu);
	m_pDLGTabMenu = new CKTDGUIDialog(m_pStage, L"DLG_Chatbox_Popup.lua");
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTabMenu );
	//}}임시

	if( NULL == m_pDLGTabMenu )
		return;

	m_ePopupWindowMode = eChatWinMode;

	//백그라운드 사이즈 지정 및 필터 목록 갱신
	//CX2ChatBox::CHAT_WINDOW_MODE eChatWinMode = GetChatWindowMode();
	float iBgHeight = 120.f; 
#ifdef ADDED_RELATIONSHIP_SYSTEM
	iBgHeight += 16.f; // 인연 필터링 추가로 1칸 증가
#endif // ADDED_RELATIONSHIP_SYSTEM

	//필터 목록 초기화
	D3DXVECTOR2 offset(0.f,0.f);
	CKTDGUIStatic* pStaticNormal =  (CKTDGUIStatic*)m_pDLGTabMenu->GetControl(L"StaticNormal"); //일반채팅
	CKTDGUICheckBox* pCheckNormal =  (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterNomal"); //일반체크박스
	if( NULL != pStaticNormal && NULL != pCheckNormal )
	{
		pStaticNormal->SetOffsetPos(offset);
		pStaticNormal->SetShowEnable(true, true);
		pCheckNormal->SetOffsetPos(offset);
		pCheckNormal->SetShowEnable(true, true);
	}
	CKTDGUIStatic* pStaticGuild =  (CKTDGUIStatic*)m_pDLGTabMenu->GetControl(L"StaticGuild"); //길드채팅
	CKTDGUICheckBox* pCheckGuild =  (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterGuild"); //길드체크박스
	if( NULL != pStaticGuild  && NULL != pCheckGuild )
	{
		pStaticGuild->SetOffsetPos(offset);
		pStaticGuild->SetShowEnable(true, true);
		pCheckGuild->SetOffsetPos(offset);
		pCheckGuild->SetShowEnable(true, true);
	}
	CKTDGUIStatic* pStaticParty =  (CKTDGUIStatic*)m_pDLGTabMenu->GetControl(L"StaticParty"); //파티채팅
	CKTDGUICheckBox* pCheckParty =  (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterParty"); //파티체크박스
	if( NULL != pStaticParty && NULL != pCheckParty )
	{
		pStaticParty->SetOffsetPos(offset);
		pStaticParty->SetShowEnable(true, true);
		pCheckParty->SetOffsetPos(offset);
		pCheckParty->SetShowEnable(true, true);
	}
	CKTDGUIStatic* pStaticWhisper =  (CKTDGUIStatic*)m_pDLGTabMenu->GetControl(L"StaticWhisper"); //귓속말채팅
	CKTDGUICheckBox* pCheckWhisper =  (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterWhisper"); //귓속말체크박스
	if( NULL != pStaticWhisper && NULL != pCheckWhisper )
	{
		pStaticParty->SetOffsetPos(offset);
		pStaticParty->SetShowEnable(true, true);
		pCheckWhisper->SetOffsetPos(offset);
		pCheckWhisper->SetShowEnable(true, true);
	}

#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIStatic* pStaticRelationCouple =  static_cast<CKTDGUIStatic*> ( m_pDLGTabMenu->GetControl(L"StaticRelationCouple") ); //귓속말채팅
	CKTDGUIStatic* pStaticRelationMarried =  static_cast<CKTDGUIStatic*> ( m_pDLGTabMenu->GetControl(L"StaticRelationMarried") ); //귓속말채팅
	CKTDGUICheckBox* pCheckRelation =  static_cast<CKTDGUICheckBox*> ( m_pDLGTabMenu->GetControl(L"FilterRelation") ); //귓속말체크박스
	
	if( NULL != pStaticWhisper && NULL != pCheckWhisper )
	{
		if ( NULL != g_pData->GetRelationshipManager() &&
			NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
		{
			switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
			{
			case SEnum::RT_COUPLE :	
				pStaticRelationCouple->SetShowEnable(true, true);
				pStaticRelationMarried->SetShowEnable(false, false);
				break;
			case SEnum::RT_MARRIED : 
				pStaticRelationMarried->SetShowEnable(true, true);
				pStaticRelationCouple->SetShowEnable(false, false);
				break;
			case SEnum::RT_SOLO : 
				break;

			}
		
		}	

		pStaticRelationCouple->SetOffsetPos(offset);
		pStaticRelationMarried->SetOffsetPos(offset);
		
		pCheckRelation->SetOffsetPos(offset);
		pCheckRelation->SetShowEnable(true, true);
	}

#endif // ADDED_RELATIONSHIP_SYSTEM
	switch( eChatWinMode )
	{
	case CWM_PARTY:
		{
			if( NULL != pStaticNormal && NULL != pStaticGuild && NULL != pStaticParty && NULL != pStaticWhisper
				&& NULL != pCheckNormal && NULL!= pCheckGuild && NULL != pCheckParty && NULL != pCheckWhisper 
#ifdef ADDED_RELATIONSHIP_SYSTEM	
				&& NULL != pStaticRelationCouple && NULL != pCheckRelation && NULL != pStaticRelationMarried
#endif // ADDED_RELATIONSHIP_SYSTEM
			  )
			{
				//이미지
				pStaticNormal->SetShowEnable(false,false);
				pStaticGuild->SetShowEnable(false,false);

				//체크박스
				pCheckNormal->SetShowEnable(false,false);
				pCheckGuild->SetShowEnable(false,false);

				offset.y -= 16.f;
				pStaticParty->SetOffsetPos(offset); //이미지
				pCheckParty->SetOffsetPos(offset); //체크박스
				offset.y -= 16.f;
				pStaticWhisper->SetOffsetPos(offset);//이미지
				pCheckWhisper->SetOffsetPos(offset); //체크박스

				iBgHeight -= 32.f;
#ifdef ADDED_RELATIONSHIP_SYSTEM
				pStaticRelationCouple->SetShowEnable(false,false);
				pStaticRelationMarried->SetShowEnable(false,false);
				pCheckRelation->SetShowEnable(false,false);
				iBgHeight -= 16.f;
#endif // ADDED_RELATIONSHIP_SYSTEM
			}
		}
		break;
	case CWM_GUILD:
		{
			if( NULL != pStaticNormal && NULL != pStaticGuild && NULL != pStaticParty && NULL != pStaticWhisper
				&& NULL != pCheckNormal && NULL!= pCheckGuild && NULL != pCheckParty && NULL != pCheckWhisper 	)
			{
				pStaticNormal->SetShowEnable(false,false);
				pStaticParty->SetShowEnable(false,false);
				//체크박스
				pCheckNormal->SetShowEnable(false,false);
				pCheckParty->SetShowEnable(false,false);

				offset.y -= 30.f;
				pStaticGuild->SetOffsetPos(offset); //이미지
				pStaticWhisper->SetOffsetPos(offset); //이미지
				pCheckGuild->SetOffsetPos(offset); //체크박스
				pCheckWhisper->SetOffsetPos(offset); //체크박스

				iBgHeight -= 32.f;
#ifdef ADDED_RELATIONSHIP_SYSTEM
				pStaticRelationCouple->SetShowEnable(false,false);
				pStaticRelationMarried->SetShowEnable(false,false);
				pCheckRelation->SetShowEnable(false,false);
				iBgHeight -= 16.f;
#endif // ADDED_RELATIONSHIP_SYSTEM


			}
		}
		break;

	case CWM_PERSONAL:
		{
			if( NULL != pStaticNormal && NULL != pStaticGuild && NULL != pStaticParty && NULL != pStaticWhisper
				&& NULL != pCheckNormal && NULL!= pCheckGuild && NULL != pCheckParty && NULL != pCheckWhisper 	)
			{
				pStaticNormal->SetShowEnable(false,false);
				pStaticParty->SetShowEnable(false,false);
				pStaticGuild->SetShowEnable(false,false);
				//체크박스
				pCheckNormal->SetShowEnable(false,false);
				pCheckParty->SetShowEnable(false,false);
				pCheckGuild->SetShowEnable(false,false);

				offset.y -= 46.f;
				pStaticWhisper->SetOffsetPos(offset); //이미지
				pCheckWhisper->SetOffsetPos(offset); //체크박스
				iBgHeight -= 48.f;

#ifdef ADDED_RELATIONSHIP_SYSTEM
				pStaticRelationCouple->SetShowEnable(false,false);
				pStaticRelationMarried->SetShowEnable(false,false);

				iBgHeight -= 16.f;
#endif // ADDED_RELATIONSHIP_SYSTEM

			}
		}
		break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CWM_RELATION_MARRIED:
	case CWM_RELATION_COUPLE:
		{
			if( NULL != pStaticNormal && NULL != pStaticGuild && NULL != pStaticParty && NULL != pStaticWhisper
				&& NULL != pCheckNormal && NULL!= pCheckGuild && NULL != pCheckParty && NULL != pCheckWhisper 	)
			{
				pStaticNormal->SetShowEnable(false,false);
				pStaticParty->SetShowEnable(false,false);
				pStaticGuild->SetShowEnable(false, false);
				//체크박스
				pCheckNormal->SetShowEnable(false,false);
				pCheckParty->SetShowEnable(false,false);
				pCheckGuild->SetShowEnable(false, false);

				offset.y -= 46.f;
				
				if ( NULL != g_pData->GetRelationshipManager() &&
					NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
				{
					switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
					{
					case SEnum::RT_COUPLE :	
						pStaticRelationMarried->SetShowEnable( false, false );
						pStaticRelationCouple->SetOffsetPos(offset); //이미지
						iBgHeight -= 16.f;	
						break;
					case SEnum::RT_MARRIED : 
						pStaticRelationCouple->SetShowEnable( false, false );
						pStaticRelationMarried->SetOffsetPos(offset); //이미지
						iBgHeight -= 16.f;	
						break;
					}
				}	
				pCheckRelation->SetOffsetPos(offset);
				pStaticWhisper->SetOffsetPos(offset); //이미지
				pCheckRelation->SetOffsetPos(offset); //체크박스
				pCheckWhisper->SetOffsetPos(offset); //체크박스

				iBgHeight -= 32.f;	
			}
		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	default:
		break;
	}
	CKTDGUIStatic* pStaticPopup =  (CKTDGUIStatic*)m_pDLGTabMenu->GetControl(L"Chatbox_Popup");
	if( NULL != pStaticPopup && NULL != pStaticPopup->GetPicture(0) )
		pStaticPopup->GetPicture(0)->SetSizeY(iBgHeight);


	// 팝업메뉴 띄울 위치 지정
	D3DXVECTOR2 vMousePos;
	vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
	vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );
	if( (vMousePos.x-40.f) < 40.f )
	{
		vMousePos.x = 40.f;
	}
	m_pDLGTabMenu->SetPos( D3DXVECTOR2( (float)vMousePos.x-40.f, 540.f ));
	m_pDLGTabMenu->SetFront(true);
	m_pDLGTabMenu->SetShowEnable(true, true);

	//체크 갱신
	SetCheckBox(eChatWinMode);
}
void CX2ChatBox::SetCheckBox( CX2ChatBox::CHAT_WINDOW_MODE eChatWinMode )
{

	CKTDGUICheckBox* pCheckExp = (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterExp"); //경험치
	if( NULL != pCheckExp && false == pCheckExp->GetEnable())
	{	//현재 활성화 되지 않은 상태라면 NULL로 변경.
		pCheckExp = NULL;
	}
	CKTDGUICheckBox* pCheckED = (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterED"); //ED
	if( NULL != pCheckED && false == pCheckED->GetEnable())
	{
		pCheckED = NULL;
	}
	CKTDGUICheckBox* pCheckSystem = (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterSystem"); //시스템
	if( NULL != pCheckSystem && false == pCheckSystem->GetEnable())
	{
		pCheckSystem = NULL;
	}
	CKTDGUICheckBox* pCheckNormal = (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterNomal"); //일반
	if( NULL != pCheckNormal && false == pCheckNormal->GetEnable())
	{
		pCheckNormal = NULL;
	}
	CKTDGUICheckBox* pCheckParty = (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterParty"); //파티
	if( NULL != pCheckParty && false == pCheckParty->GetEnable())
	{
		pCheckParty = NULL;
	}
	CKTDGUICheckBox* pCheckGuild = (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterGuild"); //길드
	if( NULL != pCheckGuild && false == pCheckGuild->GetEnable())
	{
		pCheckGuild = NULL;
	}
	CKTDGUICheckBox* pCheckWhisper = (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterWhisper"); //귓속말
	if( NULL != pCheckWhisper && false == pCheckWhisper->GetEnable())
	{
		pCheckWhisper = NULL;
	}

#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUICheckBox* pCheckRelation = (CKTDGUICheckBox*)m_pDLGTabMenu->GetControl(L"FilterRelation"); // 인연
	if( NULL != pCheckRelation && false == pCheckRelation->GetEnable())
	{
		pCheckRelation = NULL;
	}

#endif // ADDED_RELATIONSHIP_SYSTEM

	switch(eChatWinMode)
	{
	case CWM_TOTAL:
		{
			if( NULL != pCheckExp )
				pCheckExp->SetChecked( m_mapChatOptionInfo[COI_TOTAL_EXP] );

			if( NULL != pCheckED )
				pCheckED->SetChecked( m_mapChatOptionInfo[COI_TOTAL_ED] );

			if( NULL != pCheckSystem )
				pCheckSystem->SetChecked( m_mapChatOptionInfo[COI_TOTAL_SYSTEM] );

			if( NULL != pCheckNormal )
				pCheckNormal->SetChecked( m_mapChatOptionInfo[COI_TOTAL_NORMAL] );

			if( NULL != pCheckParty )
				pCheckParty->SetChecked( m_mapChatOptionInfo[COI_TOTAL_PARTY] );

			if( NULL != pCheckGuild )
				pCheckGuild->SetChecked( m_mapChatOptionInfo[COI_TOTAL_GUILD] );

			if( NULL != pCheckWhisper )
				pCheckWhisper->SetChecked( m_mapChatOptionInfo[COI_TOTAL_WHISPER] );
#ifdef ADDED_RELATIONSHIP_SYSTEM
			if( NULL != pCheckRelation )
				pCheckRelation->SetChecked( m_mapChatOptionInfo[COI_TOTAL_RELATION] );
#endif // #ifdef ADDED_RELATIONSHIP_SYSTEM
		} break;
	case CWM_PARTY:
		{
			if( NULL != pCheckExp )
				pCheckExp->SetChecked( m_mapChatOptionInfo[COI_PARTY_EXP] );

			if( NULL != pCheckED )
				pCheckED->SetChecked( m_mapChatOptionInfo[COI_PARTY_ED] );

			if( NULL != pCheckSystem )
				pCheckSystem->SetChecked( m_mapChatOptionInfo[COI_PARTY_SYSTEM] );

			if( NULL != pCheckParty )
				pCheckParty->SetChecked( m_mapChatOptionInfo[COI_PARTY_PARTY] );

			if( NULL != pCheckWhisper )
				pCheckWhisper->SetChecked( m_mapChatOptionInfo[COI_PARTY_WHISPER] );
		} break;
	case CWM_GUILD:
		{
			if( NULL != pCheckExp )
				pCheckExp->SetChecked( m_mapChatOptionInfo[COI_GUILD_EXP] );

			if( NULL != pCheckED )
				pCheckED->SetChecked( m_mapChatOptionInfo[COI_GUILD_ED] );

			if( NULL != pCheckSystem )
				pCheckSystem->SetChecked( m_mapChatOptionInfo[COI_GUILD_SYSTEM] );

			if( NULL != pCheckGuild )
				pCheckGuild->SetChecked( m_mapChatOptionInfo[COI_GUILD_GUILD] );

			if( NULL != pCheckWhisper )
				pCheckWhisper->SetChecked( m_mapChatOptionInfo[COI_GUILD_WHISPER] );

		} break;
	case CWM_PERSONAL:
		{
			if( NULL != pCheckExp )
				pCheckExp->SetChecked( m_mapChatOptionInfo[COI_WHISPER_EXP] );

			if( NULL != pCheckED )
				pCheckED->SetChecked( m_mapChatOptionInfo[COI_WHISPER_ED] );

			if( NULL != pCheckSystem )
				pCheckSystem->SetChecked( m_mapChatOptionInfo[COI_WHISPER_SYSTEM] );

			if( NULL != pCheckWhisper )
				pCheckWhisper->SetChecked( m_mapChatOptionInfo[COI_WHISPER_WHISPER] );

		} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CWM_RELATION_MARRIED:
	case CWM_RELATION_COUPLE:
		{
			if( NULL != pCheckExp )
				pCheckExp->SetChecked( m_mapChatOptionInfo[COI_RELATION_EXP] );

			if( NULL != pCheckED )
				pCheckED->SetChecked( m_mapChatOptionInfo[COI_RELATION_ED] );

			if( NULL != pCheckSystem )
				pCheckSystem->SetChecked( m_mapChatOptionInfo[COI_RELATION_SYSTEM] );

			if( NULL != pCheckWhisper )
				pCheckWhisper->SetChecked( m_mapChatOptionInfo[COI_RELATION_WHISPER] );

			if( NULL != pCheckRelation )
				pCheckRelation->SetChecked( m_mapChatOptionInfo[COI_RELATION_RELATION] );

		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM

	default:
		break;
	}
	
	/*
	
		switch(eChatWinMode)
		{
		case CWM_TOTAL:
			break;
		case CWM_PARTY:
			break;
		case CWM_GUILD:
			break;
		case CWM_PERSONAL:
			break;
		default:
			break;
		}*/
	
}
void CX2ChatBox::SetCheckBoxData( CHAT_OPTION_INFO eChatOptionInfo )
{
	std::map<byte, bool>::iterator it = m_mapChatOptionInfo.find( eChatOptionInfo );
	if( it != m_mapChatOptionInfo.end() )
	{
		it->second = (!it->second);
		Handler_EGS_CHAT_OPTION_INFO_WRITE_REQ(it->first, it->second);
	}
}
const bool CX2ChatBox::GetDisplayObtainingED( CHAT_WINDOW_MODE eChatWindowMode )
{
	switch( eChatWindowMode )
	{
	case CX2ChatBox::CWM_TOTAL :
		{
			return m_mapChatOptionInfo[COI_TOTAL_ED];
		}
		break;
	case CX2ChatBox::CWM_PARTY :
		{
			return m_mapChatOptionInfo[COI_PARTY_ED];
		}
		break;
	case CX2ChatBox::CWM_GUILD :
		{
			return m_mapChatOptionInfo[COI_GUILD_ED];
		}
		break;
	case CX2ChatBox::CWM_PERSONAL :
		{
			return m_mapChatOptionInfo[COI_WHISPER_ED];
		}
		break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2ChatBox::CWM_RELATION_COUPLE :
	case CX2ChatBox::CWM_RELATION_MARRIED :
		{
			return m_mapChatOptionInfo[COI_RELATION_ED];
		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM

	default:
		return false;
		break;
	}
}
const bool CX2ChatBox::GetDisplayObtainingEXP( CHAT_WINDOW_MODE eChatWindowMode /*= CHAT_WINDOW_MODE::CWM_CURRENT*/ )
{
	switch( eChatWindowMode )
	{
	case CX2ChatBox::CWM_TOTAL :
		{
			return m_mapChatOptionInfo[COI_TOTAL_EXP];
		}
		break;
	case CX2ChatBox::CWM_PARTY :
		{
			return m_mapChatOptionInfo[COI_PARTY_EXP];
		}
		break;
	case CX2ChatBox::CWM_GUILD :
		{
			return m_mapChatOptionInfo[COI_GUILD_EXP];
		}
		break;
	case CX2ChatBox::CWM_PERSONAL :
		{
			return m_mapChatOptionInfo[COI_WHISPER_EXP];
		}
		break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2ChatBox::CWM_RELATION_COUPLE :
	case CX2ChatBox::CWM_RELATION_MARRIED :
		{
			return m_mapChatOptionInfo[COI_RELATION_EXP];
		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	default:
		return false;
		break;
	}
}
const bool CX2ChatBox::GetDisplaySystem( CHAT_WINDOW_MODE eChatWindowMode /*= CHAT_WINDOW_MODE::CWM_CURRENT*/ )
{
	switch( eChatWindowMode )
	{
	case CX2ChatBox::CWM_TOTAL :
		{
			return m_mapChatOptionInfo[COI_TOTAL_SYSTEM];
		}
		break;
	case CX2ChatBox::CWM_PARTY :
		{
			return m_mapChatOptionInfo[COI_PARTY_SYSTEM];
		}
		break;
	case CX2ChatBox::CWM_GUILD :
		{
			return m_mapChatOptionInfo[COI_GUILD_SYSTEM];
		}
		break;
	case CX2ChatBox::CWM_PERSONAL :
		{
			return m_mapChatOptionInfo[COI_WHISPER_SYSTEM];
		}
		break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2ChatBox::CWM_RELATION_COUPLE :
	case CX2ChatBox::CWM_RELATION_MARRIED :
	{
			return m_mapChatOptionInfo[COI_RELATION_SYSTEM];
		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	default:
		return false;
		break;
	}
}
const bool CX2ChatBox::GetDisplayNomal( CHAT_WINDOW_MODE eChatWindowMode /*= CHAT_WINDOW_MODE::CWM_CURRENT*/ )
{
	switch( eChatWindowMode )
	{
	case CX2ChatBox::CWM_TOTAL :
		{
			return m_mapChatOptionInfo[COI_TOTAL_NORMAL];
		}
		break;
	default:
		return false;
		break;
	}
}
const bool CX2ChatBox::GetDisplayParty( CHAT_WINDOW_MODE eChatWindowMode /*= CHAT_WINDOW_MODE::CWM_CURRENT*/ )
{
	switch( eChatWindowMode )
	{
	case CX2ChatBox::CWM_TOTAL :
		{
			return m_mapChatOptionInfo[COI_TOTAL_PARTY];
		}
		break;
	case CX2ChatBox::CWM_PARTY :
		{
			return m_mapChatOptionInfo[COI_PARTY_PARTY];
		}
		break;
	default:
		return false;
		break;
	}
}
const bool CX2ChatBox::GetDisplayGuild( CHAT_WINDOW_MODE eChatWindowMode /*= CHAT_WINDOW_MODE::CWM_CURRENT*/ )
{
	switch( eChatWindowMode )
	{
	case CX2ChatBox::CWM_TOTAL :
		{
			return m_mapChatOptionInfo[COI_TOTAL_GUILD];
		}
		break;
	case CX2ChatBox::CWM_GUILD :
		{
			return m_mapChatOptionInfo[COI_GUILD_GUILD];
		}
		break;
	default:
		return false;
		break;
	}
}


#ifdef ADDED_RELATIONSHIP_SYSTEM

const bool CX2ChatBox::GetDisplayRelation( CHAT_WINDOW_MODE eChatWindowMode /*= CHAT_WINDOW_MODE::CWM_CURRENT*/ )
{
	switch( eChatWindowMode )
	{
	case CX2ChatBox::CWM_TOTAL :
		{
			return m_mapChatOptionInfo[COI_TOTAL_RELATION];
		}
		break;
	case CX2ChatBox::CWM_PERSONAL :
		{
			return m_mapChatOptionInfo[COI_WHISPER_RELATION];
		}
		break;
	case CX2ChatBox::CWM_RELATION_COUPLE :
	case CX2ChatBox::CWM_RELATION_MARRIED :
	case CX2ChatBox::CWM_CURRENT :
		{
			return m_mapChatOptionInfo[COI_RELATION_RELATION];
		}
		break;
	default:
		return false;
		break;
	}
}

#endif // ADDED_RELATIONSHIP_SYSTEM


const bool CX2ChatBox::GetDisplayWhisper( CHAT_WINDOW_MODE eChatWindowMode /*= CHAT_WINDOW_MODE::CWM_CURRENT*/ )
{
	switch( eChatWindowMode )
	{
	case CX2ChatBox::CWM_TOTAL :
		{
			return m_mapChatOptionInfo[COI_TOTAL_WHISPER];
		}
		break;
	case CX2ChatBox::CWM_PARTY :
		{
			return m_mapChatOptionInfo[COI_PARTY_WHISPER];
		}
		break;
	case CX2ChatBox::CWM_GUILD :
		{
			return m_mapChatOptionInfo[COI_GUILD_WHISPER];
		}
		break;
	case CX2ChatBox::CWM_PERSONAL :
		{
			return m_mapChatOptionInfo[COI_WHISPER_WHISPER];
		}
		break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2ChatBox::CWM_RELATION_COUPLE :
	case CX2ChatBox::CWM_RELATION_MARRIED :
		{
			return m_mapChatOptionInfo[COI_RELATION_WHISPER];
		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	default:
		return false;
		break;
	}
}

void CX2ChatBox::ListBoxAddItem(wstring ControlName, wstring chatContent, int addRow )
{
	const float MAGIC_CHAT_SHOW_TIME = 50.0f;
	const int MAX_LINE = 100;

	CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGChatWindow->GetControl( ControlName.c_str() );
	if( NULL != pListBox )
	{
		const int iLastTrackPos = pListBox->GetScrollBar()->GetTrackPos() + pListBox->GetScrollBar()->GetPageSize();
		const int iLastTrackEndPos = pListBox->GetScrollBar()->GetTrackEndPos();

		while( pListBox->GetSize() > MAX_LINE )
		{
			pListBox->RemoveItem(0);
		}

		pListBox->AddItem( chatContent.c_str(), NULL );
		CKTDGUIListBox::ListBoxItem* pItem = pListBox->GetLastItem();

		if( NULL != pItem )
		{
			pItem->m_bShow = true;
			pItem->m_fTimeLeftShow = MAGIC_CHAT_SHOW_TIME; 
		} 

		for( int i = 0; i < addRow; i++ )
		{
			pListBox->AddItem( L"", NULL );
		}


		if( iLastTrackPos == iLastTrackEndPos ) 
		{
			pListBox->SetScrollBarEndPos();
		}
	}
}
void CX2ChatBox::SetOptionData(std::map<byte,bool>mapChatOptionInfo)
{	
	m_mapChatOptionInfo.clear();
	for( int i=0; i< COI_END; i++)
	{
		m_mapChatOptionInfo[i] = true;
	}		

	
	for( std::map<byte, bool>::iterator it = mapChatOptionInfo.begin();	it != mapChatOptionInfo.end(); it++)
	{
		std::map<byte, bool>::iterator itData = m_mapChatOptionInfo.find (it->first);
		if( itData != m_mapChatOptionInfo.end() ) 
		{
			itData->second = it->second;
		}
	}
}

void CX2ChatBox::ClearSelectChatListBox()
{
	CKTDGUIListBox* pListBox = NULL;
	switch( GetChatWindowMode() )
	{
	case CWM_TOTAL:
		{
			pListBox = static_cast<CKTDGUIListBox*>(m_pDLGChatWindow->GetControl( L"ListBox_All"));
		} break;
	case CWM_PARTY:
		{
			pListBox = static_cast<CKTDGUIListBox*>(m_pDLGChatWindow->GetControl( L"ListBox_Party"));
		} break;
	case CWM_PERSONAL:
		{
			pListBox = static_cast<CKTDGUIListBox*>(m_pDLGChatWindow->GetControl( L"ListBox_Personal"));
		} break;
	case CWM_GUILD:
		{
			pListBox = static_cast<CKTDGUIListBox*>(m_pDLGChatWindow->GetControl( L"ListBox_Guild"));
		} break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CWM_RELATION_MARRIED:
	case CWM_RELATION_COUPLE:
		{
			pListBox = static_cast<CKTDGUIListBox*>(m_pDLGChatWindow->GetControl( L"ListBox_Relation"));
		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	default:
		{
			pListBox = static_cast<CKTDGUIListBox*>(m_pDLGChatWindow->GetControl( L"ListBox_All"));
		} break;
	}

	if( NULL != pListBox )
		pListBox->ClearSelect();
}

void CX2ChatBox::PopupNasodScopeUserMenu(bool bPopup_)
{
	CKTDGUIListBox* pListBox = static_cast<CKTDGUIListBox*>(m_pDLGChatWindow->GetControl( L"ListBox_Megaphone"));
	if( NULL == pListBox )
		return;

	const int iSelectedIndex = pListBox->GetSelectedIndex();		
	if( iSelectedIndex < 0 )
		return;

	CKTDGUIListBox::ListBoxItem* pSelectedItem = pListBox->GetItem(iSelectedIndex);
	if( NULL == pSelectedItem )
		return;

	//메가폰을 보낸 케릭터 이름 받아오기
	m_wstrPickedUserNickName = (wchar_t *)pSelectedItem->GetSenderName()->c_str();	
		
	//메가폰을 보낸 캐릭터가 자기 이름이라면 팝업 띄우지 않기
	if( 
#ifdef NASOD_SCOPE_USER_NULLCHECK
		NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() &&
#endif //NASOD_SCOPE_USER_NULLCHECK
		m_wstrPickedUserNickName == g_pData->GetMyUser()->GetSelectUnit()->GetNickName())
		return;

	if(pSelectedItem->GetMessageID() > -1)
	{
		//	인덱스와 같은 번호를 선택되게 설정
		for(int i = 0; i < pListBox->GetSize(); i++)
		{
			CKTDGUIListBox::ListBoxItem *pLoopListBox = pListBox->GetItem(i);
			if( pSelectedItem->GetMessageID() == pLoopListBox->GetMessageID() )
			{
				pLoopListBox->SetMegaSelect( true );
			}
			else
			{
				pLoopListBox->SetMegaSelect( false );
			}
		}

		if( false == bPopup_ )
			return;
		////	팝업 메뉴 띄우기
		m_pDLGNasodScopeUserMenu->SetFront(true);

		// 팝업메뉴 띄울 위치 지정
		D3DXVECTOR2 vMousePos;
		vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
		vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
		vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );

		// 팝업 메뉴가 화면 밖으로 벗어나지 않게 수정
		if( vMousePos.x > 860.f )
			vMousePos.x = 860.f;

		if( vMousePos.x < 0.f )
			vMousePos.x = 0.f;

		if( vMousePos.y > 700.f )
			vMousePos.y = 700.f;

		if( vMousePos.y < 0.f )
			vMousePos.y = 0.f;

		m_pDLGNasodScopeUserMenu->SetPos( D3DXVECTOR2( (float)vMousePos.x-30.f, (float)vMousePos.y ) );
		m_pDLGNasodScopeUserMenu->SetShowEnable(true, true);
	}
}

void CX2ChatBox::PopTalkBox( const KEGS_CHAT_NOT& kEvent_,	const WCHAR* pWstrMsg_, 
	const D3DXCOLOR& coTextColor_ /*= D3DXCOLOR(0,0,0,1)*/, const D3DXCOLOR& coOutColor_ /*= (DWORD)0x00000000*/, const D3DXCOLOR& coBackColor_ /*= (DWORD)0xffffffff*/ )
{
	CX2State* pX2StateNow = static_cast<CX2State*>( g_pMain->GetNowState() );
	if ( NULL != pX2StateNow )
	{
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
		case CX2Main::XS_SQUARE_GAME:
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_BATTLE_FIELD:
		case CX2Main::XS_TRAINING_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
		case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
			{
				pX2StateNow->PopTalkBox( kEvent_.m_SenderUnitUID, pWstrMsg_, coTextColor_, coOutColor_, coBackColor_ );

			} break;

		case CX2Main::XS_PVP_ROOM:
		case CX2Main::XS_PVP_GAME:
			{
				//{{ kimhc // 2010.3.11 //	채팅창 개선
#ifdef	CHAT_WINDOW_IMPROV
				if ( kEvent_.m_cChatPacketType == KEGS_CHAT_REQ::CPT_TOTAL 
					&& kEvent_.m_cRoomChatType == CX2Room::RCT_TEAM )
				{
					D3DXCOLOR coTeamBackColor(0.427f, 0.812f, 0.965f, 1);			// 109, 207, 246
					pX2StateNow->PopTalkBox( kEvent_.m_SenderUnitUID, pWstrMsg_, coTextColor_, coOutColor_, coTeamBackColor );
				}
				else
#endif	CHAT_WINDOW_IMPROV
					//}} kimhc // 2010.3.11 //	채팅창 개선
					pX2StateNow->PopTalkBox( kEvent_.m_SenderUnitUID, pWstrMsg_, coTextColor_, coOutColor_, coBackColor_ );

			} break;

		default:
			break;
		}
	}
}

#ifdef CHAT_LOG_TO_TXT
void CX2ChatBox::AddChatLogToFile( const WCHAR* wszMsg )
{
	if( NULL != g_pData && NULL != g_pData->GetMyUser() && CX2User::XUAL_OPERATOR <= g_pData->GetMyUser()->GetAuthLevel() )
		KLOG("ChatLog.txt") << wszMsg << "\t" << fileout;
}
#endif // CHAT_LOG_TO_TXT