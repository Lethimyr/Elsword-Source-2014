#include "StdAfx.h"
#include "X2GuildManager.h"

//{{ 허상형 : [2009/9/15] //	길드매니저 클래스 함수 정의
#ifdef GUILD_MANAGEMENT

CX2GuildManager::CX2GuildManager() :
m_pUIGuild(NULL),
m_pMyGuildInfo(NULL),
m_iUseGuildItemUID(0),
m_wstrSelectedUserName(L""),
m_iSelectedUserUID(0),
m_pDlgChangeGuildMessage(NULL),
m_pDlgNewMaster(NULL),
m_pDlgNewMasterRe(NULL),
m_wstrCreateGuildName(L""),
m_bIsShownGuildMessage(false),
m_byLimitGuildLevel( 0 ),
#ifdef GUILD_SKILL
m_ucLimitGuildGrade(_CONST_GUILD_MANAGER_::g_ucLimitGuildGrade), // oasis907 : 김상윤 [2009.11.18] //
#endif GUILD_SKILL
m_DlgKickByMaster( NULL )
{
	if( m_pUIGuild == NULL )
	{
		m_pUIGuild = new CX2UIGuild;
	}
}

CX2GuildManager::~CX2GuildManager()
{
	SAFE_DELETE( m_pUIGuild );
	SAFE_DELETE( m_pMyGuildInfo );

	m_setGuildUIDForInvitation.clear();
	m_setUidOfGuildMembers.clear();

	std::map< CKTDGUIDialogType,int >::iterator	mItr = m_mapGuildInvitationDlg.begin();

	while ( mItr != m_mapGuildInvitationDlg.end() )
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, mItr->first, NULL, false );
		mItr++;
	}
	m_mapGuildInvitationDlg.clear();

}

HRESULT CX2GuildManager::OnFrameMove( double fTime, float fElapsedTime )
{
	return S_OK;
}

bool CX2GuildManager::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef GUILD_SKILL
	//{{  [11/10/2009 김상윤]
	if(m_pUIGuild != NULL)
	{
		if ( g_pData->GetGuildManager()->GetUIGuild()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
	//}}  [11/10/2009 김상윤]
#endif GUILD_SKILL


	switch(wParam)
	{
	case GUM_GUILD_NAME_OK:		//	길드명 입력 확인 버튼
		{
			//	메시지 검사
			if( m_pUIGuild->GetDlgCreateGuild() != NULL)
			{
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pUIGuild->GetDlgCreateGuild()->GetControl( L"IME_Guildname" );
				if(pIMEEditBox != NULL)
				{
					wstring wszGuildName = pIMEEditBox->GetText();

					//	메시지 입력이 없을 경우
					if( wcscmp(pIMEEditBox->GetText(), L"") == 0 )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4493 ) , g_pMain->GetNowState() );
						return true;
					}

					//	금지어가 포함 되어 있을 경우
					if( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_CHAT, wszGuildName.c_str() ) == false )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4492, "L", wszGuildName ) ), g_pMain->GetNowState() );

						return true;
					}

					//{{ 허상형 : [2009/11/5] //	닉네임 필터 추가
					//	금지어가 포함 되어 있을 경우
					if( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, wszGuildName.c_str() ) == false )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4492, "L", wszGuildName ) ), g_pMain->GetNowState() );

						return true;
					}

					//	다시 한번 물어봄
					m_wstrCreateGuildName = wszGuildName;
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250, 300), GET_REPLACED_STRING( ( STR_ID_4609, "L", wszGuildName ) ), GUM_GUILD_CREATE_OK, g_pMain->GetNowState() );
					
					return true;
				}
			}

			return false;
		}
		break;
	case GUM_GUILD_NAME_CANCEL:	//	길드명 입력 취소 버튼
		{
			m_pUIGuild->SetShowCreateGuildDlg( false );
			return true;
		}
		break;

#ifdef SERV_GUILD_CHANGE_NAME 
	case GUM_GUILD_NAME_CHANGE_CHECK_OK:
		{ 
			if( m_pUIGuild->GetDlgChangeGuildName() == NULL ) 
				return false;
			CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*) m_pUIGuild->GetDlgChangeGuildName()->GetControl( L"IME_NewNick" );
			if( NULL != pEditBox )
			{

				wstring wstrGuildName = pEditBox->GetText();

				//	메시지 입력이 없을 경우
				if( wcscmp(pEditBox->GetText(), L"") == 0 )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4493 ) , g_pMain->GetNowState() );
					return true;
				}
				// 잘못된 이름을 입력했을 경우
				if( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, wstrGuildName.c_str() ) == false )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_717 ), g_pMain->GetNowState() );
					return false;
				} 
				//변경 가능 여부 확인
				if ( true == g_pData->GetGuildManager()->DidJoinGuild()
					&&  CX2GuildManager::GUG_MASTER == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade )
				{ 
					Handler_EGS_CHANGE_GUILD_NAME_CHECK_REQ( wstrGuildName.c_str() );
				} 
			} 
			return true;

		}
		break;
	case GUM_GUILD_NAME_CHANGE_CHECK_CANCEL:
		{ 
			m_pUIGuild->SetShowChangeGuildNameDlg(false);
			return true;
		}
		break; 
	case GUM_GUILD_NAME_CHANGE_OK:
		{
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			Handler_EGS_CHANGE_GUILD_NAME_REQ( NetError::NET_OK );
			return true;
		}
		break;
	case GUM_GUILD_NAME_CHANGE_CNACEL:
		{

			Handler_EGS_CHANGE_GUILD_NAME_REQ( NetError::ERR_GUILD_52 );
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			return true;
		}
		break;
#endif //CHANGE_GUILD_NAME
	case GUM_GUILD_CREATE_OK:
		{
			//	서버에 길드이름 패킷 전송
			Handler_EGS_CREATE_GUILD_REQ( m_wstrCreateGuildName );
			m_wstrCreateGuildName = L"";

			//	Dialog 삭제
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );			

			return true;
		}
		break;

	case GCGUM_MASTER:
		{
			m_wstrSelectedUserName  = g_pData->GetMessenger()->GetGuildMemberNickName( g_pData->GetMessenger()->GetSelectedGuildMemberUID() );
			m_iSelectedUserUID = g_pData->GetMessenger()->GetSelectedGuildMemberUID();

			m_pDlgNewMaster = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4594, "L", m_wstrSelectedUserName ) ), GUM_GUILD_NEW_GUILD_MASTER_OK,	g_pMain->GetNowState() );
			return true;
		}
		break;

	case GCGUM_SYSOP:
	case GCGUM_OLD_USER:
	case GCGUM_VIP_USER:
	case GCGUM_NORMAL_USER:
	case GCGUM_NEW_USER:
		{
			//	note : (wParam - GCGUM_MASTER) + 1 = GUILD_USER_GRADE의 등급
			Handler_EGS_CHANGE_GUILD_MEMBER_GRADE_REQ( g_pData->GetMessenger()->GetSelectedGuildMemberUID(), (wParam - GCGUM_MASTER) + 1 );
						
			//	창 닫기
			m_pUIGuild->SetShowChangeGradeMenu( false );

			return true;
		}
		break;

	case GUM_GUILD_INVITE_MEMBER_OK:		// 초대할 유저 닉네임 입력 확인
		{
			//	메시지 검사
			if( m_pUIGuild != NULL && m_pUIGuild->GetDlgInviteGuildMember() != NULL)
			{
				CKTDGUIIMEEditBox*	pIMEEditBox = NULL;
				pIMEEditBox			= static_cast< CKTDGUIIMEEditBox* >( m_pUIGuild->GetDlgInviteGuildMember()->GetControl( L"IMEEditBoxMessage" ) );
				
				if(pIMEEditBox != NULL)
				{
					wstring wstrNickName = pIMEEditBox->GetText();

					
					if( wcscmp( pIMEEditBox->GetText(), L"" ) == 0 ||	//	메시지 입력이 없을 경우
						// 금지어가 포함 되어 있을 경우
						g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_CHAT, wstrNickName.c_str() ) == false )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4541 ) , g_pMain->GetNowState() );
						return true;
					}

					//	서버에 초대할 유저의 닉네임 패킷 전송
					Handler_EGS_INVITE_GUILD_REQ( wstrNickName );

					return true;
				}
			}

			return false;

		}
		break;

	case GUM_GUILD_INVITE_MEMBER_CANCEL:	// 초대할 유저 닉네임 입력 취소
		{
			if ( m_pUIGuild != NULL )
			{
				m_pUIGuild->SetShowInviteGuildMemberDlg( false );
			}

			return true;
		}
		break;

	case GUM_GUILD_JOIN_OK:
		{
			CKTDGUIControl*		pControl	= reinterpret_cast< CKTDGUIControl* >( lParam );

			if ( pControl == NULL )
			{
				ASSERT( !"Wrong Path!" );
				return false;
			}

			if ( m_mapGuildInvitationDlg.size() == 0 )
			{
				ASSERT( !"Map size is wrong!" );
				return false;
			}

			CKTDGUIDialogType	pDlg		= pControl->GetDialog();
			int					iGuildUID	= 0;
			std::map< CKTDGUIDialogType,int >::iterator mItr = m_mapGuildInvitationDlg.begin();

			iGuildUID	= mItr->second;

			while ( mItr != m_mapGuildInvitationDlg.end() )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, mItr->first, NULL, false );
				mItr++;
			}

			m_mapGuildInvitationDlg.clear();
			m_setGuildUIDForInvitation.clear();

			// 길드 추방 메시지가 떠있다면 삭제
			if ( m_DlgKickByMaster != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_DlgKickByMaster, NULL, false );

			// 거부 취소된 것들은 패킷을 보내지 않음
			Handler_EGS_INVITE_GUILD_REPLY_REQ( iGuildUID, NetError::NET_OK ); //수락 했다고 보내야함

			return true;
		}
		break;

	case GUM_GUILD_JOIN_CANCEL:
		{
			CKTDGUIControl*		pControl	= reinterpret_cast< CKTDGUIControl* >( lParam );
			
			if ( pControl == NULL )
			{
				ASSERT( !"Wrong Path!" );
				return false;
			}

			if ( m_mapGuildInvitationDlg.size() == 0 )
			{
				ASSERT( !"Map size is wrong!" );
				return false;
			}

			CKTDGUIDialogType	pDlg	= pControl->GetDialog();
			
			//{{ kimhc // 2009-09-29 // 취소를 누른 다이얼로그 삭제			
			std::map< CKTDGUIDialogType,int >::iterator	mItr = m_mapGuildInvitationDlg.find( pDlg );

			if ( mItr != m_mapGuildInvitationDlg.end() )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, mItr->first, NULL, false );
				m_setGuildUIDForInvitation.erase( mItr->second );
				m_mapGuildInvitationDlg.erase( mItr->first );
			}
			//}} kimhc // 2009-09-29 // 취소를 누른 다이얼로그 삭제

			return true;
			return true;
		}
		break;

	case GUM_GUILD_INFO_LEAVE_BUTTON:	// 길드 탈퇴
		{
			if ( g_pData->GetMyUser() == NULL ||
				 g_pData->GetMyUser()->GetSelectUnit() == NULL )
				 return false;

			GUILD_USER_GRADE eMyGrade = static_cast< GUILD_USER_GRADE >( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade );


			switch ( eMyGrade )
			{
			case GUG_SYSOP:				//	관리자
			case GUG_OLD_USER:			//	원로 길드원
			case GUG_VIP_USER:			//	우수 길드원
			case GUG_NORMAL_USER:		//	일반 길드원
			case GUG_NEW_USER:			//	신입 길드원
				{
					if ( m_pMyGuildInfo != NULL )
					{
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4577, "L", m_pMyGuildInfo->m_wstrGuildName ) ), 
							GUM_GUILD_LEAVE_OK, g_pMain->GetNowState(), GUM_GUILD_LEAVE_CANCEL );
						return true;
					}
					
				}
				break;

			case GUG_MASTER:
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4576 ) , g_pMain->GetNowState() );
					return true;
				}
				break;
			default:
				{
					ASSERT( !L"Wrong path" );
					return false;
				}
			}

			return true;			
		}
		break;
	case GUM_GUILD_INFO_CHANGE_MESSAGE_BUTTON:	// 길드 메시지 변경 버튼
		{
			const CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();


			if( pUnitData->m_byMemberShipGrade == GUG_MASTER ||	pUnitData->m_byMemberShipGrade == GUG_SYSOP )
			{
				g_pData->GetGuildManager()->GetUIGuild()->SetShowChangeGuildMessageDlg( true );
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(300, 250), GET_STRING( STR_ID_4731 ), g_pMain->GetNowState() );
			}
			return true;
		}
		break;

	case GUM_GUILD_INFO_CHANGE_MESSAGE_BUTTON_OK:	//	길드 메시지 변경 Dialog 확인 버튼
		{
			//	메시지 검사
			if( m_pUIGuild->GetDlgChangeGuildMessage() != NULL)
			{
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pUIGuild->GetDlgChangeGuildMessage()->GetControl( L"IMEEditBoxMessage" );
				if(pIMEEditBox != NULL)
				{
					wstring wstrMessage = pIMEEditBox->GetText();

					//	금지어가 포함 시 문자열 변경
					wstrMessage = g_pMain->GetStringFilter()->FilteringChatString( wstrMessage.c_str(), L'♡' );

					//	서버에 길드이름 패킷 전송
					return Handler_EGS_CHANGE_GUILD_MESSAGE_REQ( wstrMessage );
				}
			}

			return false;
		}
		break;
	case GUM_GUILD_INFO_CHANGE_MESSAGE_BUTTON_CANCEL:	//	길드 메시지 변경 Dialog 취소 버튼
		{
			g_pData->GetGuildManager()->GetUIGuild()->SetShowChangeGuildMessageDlg( false );
			return true;
		}
		break;

	case GUM_GUILD_INFO_CLOSE:
		{
			GetUIGuild()->SetShowGuildInfo( false );
			return true;
		}
		break;
	case GUM_GUILD_LEAVE_OK:
		{
			if ( g_pData->GetMyUser() == NULL ||
				 g_pData->GetMyUser()->GetSelectUnit() == NULL )
				 return false;

			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return Handler_EGS_KICK_GUILD_MEMBER_REQ( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
		}
		break;

	case GUM_GUILD_LEAVE_CANCEL:
		{
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		}
		break;
	case GUM_GUILD_INFO_CHANGE_MESSAGE_FINAL_OK:	//	note : EGS_GUILD_INFO_NOT를 받은 후 UI를 업데이트 해줘야 하므로 OK 버튼을 눌렀을때 UI를 갱신한다.
		{
			//	길드 정보 UI에 길드 메시지 정보를 업데이트한다.
			GetUIGuild()->SetGuildInfoTab( CX2UIGuild::GIT_GUILD_INFO );

			if( NULL != m_pDlgChangeGuildMessage )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgChangeGuildMessage, NULL, false );
				m_pDlgChangeGuildMessage = NULL;
			}


			return true;
		}
		break;

	case GUM_GUILD_INFO_DESTROY_BUTTON:
		{
			if ( g_pData->GetMyUser() == NULL ||
				 g_pData->GetMyUser()->GetSelectUnit() == NULL )
				 return false;

			if ( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade !=
				 GUG_MASTER )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4585 ) , g_pMain->GetNowState() );
				return true;
			}

			if ( CanDestroyGuild() == false )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4580 ) , g_pMain->GetNowState() );

			}
			else
			{
				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4581 ),
					GUM_GUILD_DESTROY_OK, g_pMain->GetNowState(), GUM_GUILD_DESTROY_CANCEL );
			}

			return true;

		}
		break;

	case GUM_GUILD_DESTROY_OK:
		{
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return Handler_EGS_DISBAND_GUILD_REQ();			
		}
		break;

	case GUM_GUILD_DESTROY_CANCEL:
		{
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return true;
		}
		break;

	case GUM_GUILD_INFO_TAB:
		{
			GetUIGuild()->SetGuildInfoTab( CX2UIGuild::GIT_GUILD_INFO );
		}
		break;

	case GUM_GUILD_MEMBER_INFO_TAB:
		{
			GetUIGuild()->SetGuildInfoTab( CX2UIGuild::GIT_GUILD_MEMBER_INFO );
			Handler_EGS_GET_GUILD_USER_LIST_REQ();
		}
		break;

	case GUM_GUILD_SKILL_TAB:
		{
#ifdef GUILD_SKILL			
			//{{  [11/9/2009 김상윤]
			GetUIGuild()->SetGuildInfoTab( CX2UIGuild::GIT_GUILD_SKILL );
			break;
			//}}  [11/9/2009 김상윤]
#endif GUILD_SKILL
			
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(300, 250), GET_STRING( STR_ID_4722 ), g_pMain->GetNowState() );

			//	스킬 탭 비활성화
			CKTDGUIDialogType pDlgGuildInfo = GetUIGuild()->GetDlgGuildInfo();
			if( pDlgGuildInfo != NULL )
			{
				//	
				CKTDGUIRadioButton* pRadio_Guild		= static_cast<CKTDGUIRadioButton*>( pDlgGuildInfo->GetControl( L"Radio_Guild_Info" ) );	//	길드 정보 버튼
				CKTDGUIRadioButton* pRadio_GuildMember	= static_cast<CKTDGUIRadioButton*>( pDlgGuildInfo->GetControl( L"Radio_Guild_Member_Info" ) );	//	길드 멤버 정보 버튼
				CKTDGUIRadioButton* pRadio_GuildSkill	= static_cast<CKTDGUIRadioButton*>( pDlgGuildInfo->GetControl( L"RadioButtonGuild_Skill" ) );	//	길드 멤버 정보 버튼

				if( pRadio_Guild != NULL && pRadio_GuildMember != NULL && pRadio_GuildSkill != NULL )
				{
					pRadio_GuildSkill->SetChecked( false );

					switch( GetUIGuild()->GetGuildInfoTabType() )
					{
					case CX2UIGuild::GIT_GUILD_INFO:
						{
							pRadio_Guild->SetChecked( true );
						}
						break;
					case CX2UIGuild::GIT_GUILD_MEMBER_INFO:
						{
							pRadio_GuildMember->SetChecked( true );
						}
						break;
					}
				}
			}
			
		}
		break;

	case GUM_GUILD_MEMBER_INFO_CHANGE_MESSAGE:
		{
			GetUIGuild()->SetShowChangeGuildMemberMessageDlg( true );
		}
		break;

	case GUM_GUILD_MEMBER_INFO_CHANGE_MESSAGE_OK:
		{
			//	조건검사
			if( m_pUIGuild->GetDlgChangeGuildMemberMessage() != NULL)
			{
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pUIGuild->GetDlgChangeGuildMemberMessage()->GetControl( L"IMEEditBoxMessage" );
				if(pIMEEditBox != NULL)
				{
					wstring wstrMessage = pIMEEditBox->GetText();

					//	메시지 입력이 없을 경우
					if( wstrMessage == L"" )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_3742 ), g_pMain->GetNowState() );
						return true;
					}

					//	금지어가 포함 시 문자열 변경
					wstrMessage = g_pMain->GetStringFilter()->FilteringChatString( wstrMessage.c_str(), L'♡' );

					//	서버에 패킷전송
					return Handler_EGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ( wstrMessage );
				}
			}

			return false;
		}
		break;

	case GUM_GUILD_MEMBER_INFO_CHANGE_MESSAGE_CANCEL:
		{
			GetUIGuild()->SetShowChangeGuildMemberMessageDlg( false );
		}
		break;

	case GUM_GUILD_MEMBER_INFO_PAGE_LEFT:	//	길드원 정보 페이지 다운
		{
			char cPage = GetUIGuild()->GetNowMemberInfoPage();

			if( cPage > 1 )
			{
				GetUIGuild()->SetNowMemberInfoPage( cPage - 1 );
				Handler_EGS_GET_GUILD_USER_LIST_REQ();
			}

			return true;
		}
		break;
	
	case GUM_GUILD_MEMBER_INFO_PAGE_RIGHT:	//	길드원 정보 페이지 업
		{
			char cPage = GetUIGuild()->GetNowMemberInfoPage();

			if( cPage < GetUIGuild()->GetMaxMemberInfoPage() )
			{
				GetUIGuild()->SetNowMemberInfoPage( cPage + 1 );
				Handler_EGS_GET_GUILD_USER_LIST_REQ();
			}

			return true;
		}
		break;
	case GUM_GUILD_NEW_GUILD_MASTER_OK:		//	새로운 길드 마스터 임명 Dialog 확인 버튼
		{
			m_pDlgNewMasterRe = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4595 ), GUM_GUILD_NEW_GUILD_MASTER_RE_OK,	g_pMain->GetNowState(), GUM_GUILD_NEW_GUILD_MASTER_RE_CANCEL );
			
			return true;
		}
		break;

	case GUM_GUILD_NEW_GUILD_MASTER_RE_OK:		//	새로운 길드 마스터 임명 Dialog 재확인 버튼
		{
			//	패킷 전송
			Handler_EGS_CHANGE_GUILD_MEMBER_GRADE_REQ( m_iSelectedUserUID, GUG_MASTER );
            
			m_iSelectedUserUID = 0;
			m_wstrSelectedUserName = L"";

			//	창 닫기
			m_pUIGuild->SetShowChangeGradeMenu( false );

			if( NULL != m_pDlgNewMaster )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgNewMaster, NULL, false );
				m_pDlgNewMaster = NULL;
			}

			//	창 닫기
			if( NULL != m_pDlgNewMasterRe )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgNewMasterRe, NULL, false );
				m_pDlgNewMasterRe = NULL;
			}

			return true;
		}
		break;

	case GUM_GUILD_NEW_GUILD_MASTER_RE_CANCEL:
		{
			if( NULL != m_pDlgNewMaster )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgNewMaster, NULL, false );
				m_pDlgNewMaster = NULL;
			}

			//	창 닫기
			if( NULL != m_pDlgNewMasterRe )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgNewMasterRe, NULL, false );
				m_pDlgNewMasterRe = NULL;
			}

			return true;
		}
		break;

	case GUM_GUILD_MEMBER_INFO_DELETE01:		//	길드원 인사말 삭제
	case GUM_GUILD_MEMBER_INFO_DELETE02:
	case GUM_GUILD_MEMBER_INFO_DELETE03:
	case GUM_GUILD_MEMBER_INFO_DELETE04:
	case GUM_GUILD_MEMBER_INFO_DELETE05:
	case GUM_GUILD_MEMBER_INFO_DELETE06:
	case GUM_GUILD_MEMBER_INFO_DELETE07:
	case GUM_GUILD_MEMBER_INFO_DELETE08:
		{
			int iSelectedIndex = (wParam - GUM_GUILD_MEMBER_INFO_DELETE01);

			if( iSelectedIndex < static_cast<int>(m_vecGuildMemberInfo.size()) )
			{
				m_iSelectedUserUID		= m_vecGuildMemberInfo[iSelectedIndex].m_UID;

				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_4599 ), GUM_GUILD_MEMBER_INFO_DELETE_OK, g_pMain->GetNowState() );
				return true;
			}

			return false;
		}
		break;

	case GUM_GUILD_MEMBER_INFO_DELETE_OK:
		{
			wstring wstrMessage = L"";

			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return Handler_EGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ( wstrMessage, GetSelectedUserUID() );
		}
		break;

	case GUM_GUILD_NOTICE_OK:
		{
			m_pUIGuild->SetShowCreateGuildNoticeDlg( false );
		}
		break;
	}
	return false;
}

bool CX2GuildManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();
	switch( wParam )
	{
	case EGS_CREATE_GUILD_ACK:
		{
			return Handler_EGS_CREATE_GUILD_ACK( hWnd, uMsg, wParam, lParam);
			break;
		}
	case EGS_GUILD_INFO_NOT:
		{
			return Handler_EGS_GUILD_INFO_NOT( hWnd, uMsg, wParam, lParam);
			break;
		}
	case EGS_GUILD_MESSAGE_NOT:
		{
			return Handler_EGS_GUILD_MESSAGE_NOT( hWnd, uMsg, wParam, lParam);
			break;
		}

	case EGS_INVITE_GUILD_ACK:
		{
			return Handler_EGS_INVITE_GUILD_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_INVITE_GUILD_NOT:
		{
			return Handler_EGS_INVITE_GUILD_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_INVITE_GUILD_REPLY_ACK:
		{
            return Handler_EGS_INVITE_GUILD_REPLY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_INVITE_GUILD_REPLY_DENY_NOT:
		{
			return Handler_EGS_INVITE_GUILD_REPLY_DENY_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_CHANGE_GUILD_MESSAGE_ACK:
		{
			return Handler_EGS_CHANGE_GUILD_MESSAGE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_CHANGE_GUILD_MEMBER_GRADE_ACK:
		{
			return Handler_EGS_CHANGE_GUILD_MEMBER_GRADE_ACK( hWnd, uMsg, wParam, lParam );
		}

	case EGS_KICK_GUILD_MEMBER_ACK:
		{
			return Handler_EGS_KICK_GUILD_MEMBER_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_DISBAND_GUILD_ACK:
		{
			return Handler_EGS_DISBAND_GUILD_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK:
		{
			return Handler_EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_JOIN_GUILD_SUCCESS_NOT:
		{
			return Handler_EGS_JOIN_GUILD_SUCCESS_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#ifdef SERV_GUILD_CHANGE_NAME
	case EGS_CHANGE_GUILD_NAME_CHECK_ACK:
		{
			return Handler_EGS_CHANGE_GUILD_NAME_CHECK_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_CHANGE_GUILD_NAME_ACK:
		{
			return Handler_EGS_CHANGE_GUILD_NAME_ACK( hWnd, uMsg, wParam, lParam );
		}
		break; 
#endif //CHANGE_GUILD_NAME
	default:
		break;
	}
	return false;
}

std::wstring	CX2GuildManager::GetWStringGuildMemberGrade( BYTE byMemberGrade ) const
{
	std::wstring wstrMemberGrade;

	switch ( byMemberGrade )
	{
	case GUG_MASTER:
		wstrMemberGrade = GET_STRING( STR_ID_4511 );
		break;

	case GUG_SYSOP:
		wstrMemberGrade = GET_STRING( STR_ID_4512 );
		break;

	case GUG_OLD_USER:
		wstrMemberGrade = GET_STRING( STR_ID_4513 );
		break;

	case GUG_VIP_USER:
		wstrMemberGrade = GET_STRING( STR_ID_4514 );
		break;

	case GUG_NORMAL_USER:
		wstrMemberGrade = GET_STRING( STR_ID_4515 );
		break;

	case GUG_NEW_USER:
		wstrMemberGrade = GET_STRING( STR_ID_4516 );
		break;

	default:
		break;
	}

	return wstrMemberGrade;
}

#ifdef SERV_GUILD_CHANGE_NAME 
bool CX2GuildManager::Handler_EGS_CHANGE_GUILD_NAME_CHECK_REQ( wstring wstrGuildName )
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		return false;  

	KEGS_CHANGE_GUILD_NAME_CHECK_REQ kPacket;

	kPacket.m_wstrGuildName = wstrGuildName;   	

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_REQ, kPacket );
	return true;
}
bool CX2GuildManager::Handler_EGS_CHANGE_GUILD_NAME_CHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_GUILD_NAME_CHECK_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{		  
		g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_16257 ),
			GUM_GUILD_NAME_CHANGE_OK, g_pMain->GetNowState(), GUM_GUILD_NAME_CHANGE_CNACEL );
		return true;
	}

	return false;
}
bool CX2GuildManager::Handler_EGS_CHANGE_GUILD_NAME_REQ( int iOk )
{ 
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		return false;  

	KEGS_CHANGE_GUILD_NAME_REQ kPacket;
#ifdef SERV_GLOBAL_BILLING
	KEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ* kItemPacket = g_pData->GetCashShop()->GetPurchasedCashItemInfo();
	kPacket.m_ulOrderNo		= kItemPacket->m_iTransNo;
	kPacket.m_ulProductNo	= kItemPacket->m_iProductNo;
	kPacket.m_iQuantity		= 1;
#else //SERV_GLOBAL_BILLING
//{{ 오현빈 // 2012-03-14 // 캐쉬샵에서 즉시 사용 하는 아이템이므로 삭제에 필요한 정보를 보내주기 위해 추가
	KEGS_GET_PURCHASED_CASH_ITEM_REQ* kItemPacket = g_pData->GetCashShop()->GetPurchasedCashItemInfo();
	kPacket.m_ulOrderNo		= kItemPacket->m_ulOrderNo;
	kPacket.m_ulProductNo	= kItemPacket->m_ulProductNo;
	kPacket.m_iQuantity		= kItemPacket->m_iQuantity;
//}} 오현빈 // 2012-03-14 // 캐쉬샵에서 즉시 사용 하는 아이템이므로 삭제에 필요한 정보를 보내주기 위해 추가
#endif //SERV_GLOBAL_BILLING
	kPacket.m_iOK			= iOk;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_GUILD_NAME_REQ, kPacket );
	return true;
}

bool CX2GuildManager::Handler_EGS_CHANGE_GUILD_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{ 
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_GUILD_NAME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{		  
		//	길드 이름 변경 완료
		SetGuildInfo( kEvent.m_kGuildInfo );

		//  길드명 변경 메세지 박스
		wstring wstrComment = GET_REPLACED_STRING( ( STR_ID_16230, "L", kEvent.m_kGuildInfo.m_wstrGuildName ) );
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), wstrComment.c_str(), g_pMain->GetNowState() );		

		//	다이얼로그 닫기
		m_pUIGuild->SetShowChangeGuildNameDlg( false );

		//  캐쉬 인벤토리 갱신
#ifdef SERV_GLOBAL_BILLING
		g_pData->GetCashShop()->Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#else // SERV_GLOBAL_BILLING
		g_pData->GetCashShop()->Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#endif // SERV_GLOBAL_BILLING

		return true;
	} 

	return false;
}  
#endif //CHANGE_GUILD_NAME
bool	CX2GuildManager::Handler_EGS_CREATE_GUILD_REQ( const wstring& wszGuildName ) const
{
	KEGS_CREATE_GUILD_REQ kPacket;

	if( m_iUseGuildItemUID != NULL )
	{
		kPacket.m_iItemUID = m_iUseGuildItemUID;
		kPacket.m_wstrGuildName = wszGuildName;
		kPacket.m_wstrGuildMessage = GET_STRING( STR_ID_4733 );

		g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_GUILD_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_CREATE_GUILD_ACK );

		return true;
	}

	return false;
}

bool	CX2GuildManager::Handler_EGS_CREATE_GUILD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_GUILD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if(g_pMain->DeleteServerPacket( EGS_CREATE_GUILD_ACK ))
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( g_pData->GetMyUser() == NULL ||
				g_pData->GetMyUser()->GetSelectUnit() == NULL )
				return false;

			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_byMemberShipGrade = GUG_MASTER;

			//	길드 생성 완료
			SetGuildInfo( kEvent.m_kGuildInfo );

			//	인벤토리 갱신
			CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
			//	아이템 업데이트
			kInventory.UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
			g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			//	다이얼로그 닫기
			m_pUIGuild->SetShowCreateGuildDlg( false );
			
			//	길드 생성 완료 팝업 출력
			m_pUIGuild->SetShowCreateGuildNoticeDlg( true, kEvent.m_kGuildInfo.m_wstrGuildName.c_str(), g_pData->GetMyUser()->GetSelectUnit()->GetNickName() ); 
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Guild_Open.ogg", false, false );

			return true;
		}
	}

	return false;
}

bool	CX2GuildManager::Handler_EGS_GUILD_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GUILD_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_setUidOfGuildMembers.clear();
	m_setUidOfGuildMembers.insert( kEvent.m_vecMemberUnitUID.begin(), kEvent.m_vecMemberUnitUID.end() );

	//	길드 정보 갱신
	SetGuildInfo( kEvent );

#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 [2009.11.19] //
	// 길드 스킬 갱신 구현 완료 (4. 배운 길드 스킬 리스트 얻기)
	
	CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
	
	if ( NULL != pUnit )
	{
		pUnit->AccessUnitData().m_UserSkillTree.SetGuildClass(0);
		pUnit->AccessUnitData().m_UserSkillTree.SetAcquiredGuildSkill( kEvent.m_kGuildSkillInfo.m_vecGuildSkill ); // 함수 임시 변경
	
		// oasis907 : 김상윤 [2009.11.27] // 길드 스킬 포인트, 캐시 스킬 포인트 SET
		pUnit->AccessUnitData().m_iGuildSPoint = kEvent.m_kGuildSkillInfo.m_iGuildSPoint;
		pUnit->AccessUnitData().m_iGuildCSPoint = kEvent.m_kGuildSkillInfo.m_iGuildCSPoint;
		pUnit->AccessUnitData().m_iMaxGuildCSPoint = kEvent.m_kGuildSkillInfo.m_iMaxGuildCSPoint;
		pUnit->AccessUnitData().m_wstrGuildCSPointEndDate = kEvent.m_kGuildSkillInfo.m_wstrGuildCSPointEndDate;
	}

	// oasis907 : 김상윤 [2009.12.3] // 그노시스 대축복 남은 기간 표시
	if(kEvent.m_kGuildSkillInfo.m_iMaxGuildCSPoint > 0 )
	{
		wstring wstr = L"";
		wstr += GetExpirationDateDesc( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrGuildCSPointEndDate, g_pData->GetServerCurrentTime() );

		g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4800, "L", wstr ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	}

	//}} oasis907 : 김상윤 [2009.11.19] //
#endif GUILD_SKILL

	//	UnitData에 길드명 추가
	{
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_wstrGuildName = kEvent.m_kGuildInfo.m_wstrGuildName.c_str();
	}

	return true;
}

bool	CX2GuildManager::Handler_EGS_CHANGE_GUILD_MEMBER_GRADE_REQ( UidType iUID, int iGrade )
{
	KEGS_CHANGE_GUILD_MEMBER_GRADE_REQ kPacket;

	kPacket.m_iUnitUID = iUID;
	kPacket.m_ucMemberShipGrade = (UCHAR) iGrade;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_GUILD_MEMBER_GRADE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_GUILD_MEMBER_GRADE_ACK );

	return true;
}

bool	CX2GuildManager::Handler_EGS_CHANGE_GUILD_MEMBER_GRADE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_GUILD_MEMBER_GRADE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_GUILD_MEMBER_GRADE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//	팝업 출력
			wstring wszMsg;

#ifndef INT_NO_HANGUL	//	국내 
			wszMsg = GET_REPLACED_STRING( ( STR_ID_4528, "LLS", kEvent.m_wstrTargetNickName, GetWStringGuildMemberGrade( kEvent.m_ucMemberShipGrade ),
				( kEvent.m_ucMemberShipGrade == GUG_MASTER || kEvent.m_ucMemberShipGrade == GUG_SYSOP ) ? L"" : L"으" ) );
#else	//	해외
			wszMsg = GET_REPLACED_STRING( ( STR_ID_4528, "LLS", kEvent.m_wstrTargetNickName, GetWStringGuildMemberGrade( kEvent.m_ucMemberShipGrade ), L"" ) );
#endif	//	#ifndef INT_NO_HANGUL

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), wszMsg.c_str(), g_pMain->GetNowState() );

			//	등급 정보 갱신
			return g_pData->GetMessenger()->Handler_EGS_GET_GUILD_USER_LIST_REQ();
		}
	}

	return false;
}

bool	CX2GuildManager::Handler_EGS_GUILD_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GUILD_MESSAGE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pChatBox == NULL )
	{
		return false;
	}

	switch( kEvent.m_cMessageType )
	{
	case KEGS_GUILD_MESSAGE_NOT::MT_JOIN_MEMBER:			//	길드가입
		{
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4529, "L", kEvent.m_wstrNickName ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

			if ( g_pData->GetMyUser() == NULL ||
				 g_pData->GetMyUser()->GetSelectUnit() == NULL )
			{
				ASSERT( !L"Wrong!" );
				return false;
			}

			if ( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_iUnitUID )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_byMemberShipGrade = static_cast<BYTE>(kEvent.m_iMessageData);
			}

			UpdateNowNumOfGuildMember( kEvent.m_iUnitUID );
		}
		break;
	case KEGS_GUILD_MESSAGE_NOT::MT_CHANGE_MEMBER_GRADE:	//	등급변경
		{
			wstring wszMsg;

#ifndef INT_NO_HANGUL	//	국내 
			wszMsg = GET_REPLACED_STRING( ( STR_ID_4528, "LLS", kEvent.m_wstrNickName, GetWStringGuildMemberGrade( static_cast<BYTE>(kEvent.m_iMessageData) ),
				( static_cast<BYTE>(kEvent.m_iMessageData) == GUG_MASTER || static_cast<BYTE>(kEvent.m_iMessageData) == GUG_SYSOP ) ? L"" : L"으" ) );
#else	//	해외
			wszMsg = GET_REPLACED_STRING( ( STR_ID_4528, "LLS", kEvent.m_wstrNickName, GetWStringGuildMemberGrade( static_cast<BYTE>(kEvent.m_iMessageData) ), L"" ) );
#endif	//	#ifndef INT_NO_HANGUL

			if ( g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_iUnitUID )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_byMemberShipGrade = static_cast<BYTE>(kEvent.m_iMessageData);
			}

			g_pChatBox->AddChatLog( wszMsg.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );


#ifdef GUILD_SKILL
			//{{ oasis907 : 김상윤 [2009.12.8] // 길드 스킬에 의한 스탯 변경 
			if( g_pData != NULL && 
				g_pData->GetUIManager() != NULL && 
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
			}
			// oasis907 : 김상윤 [2009.12.9] // 길드 스킬 트리창 업데이트
			if( NULL != g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() )
			{
				g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->UpdateUI(true, true, true); // oasis
			}

#endif GUILD_SKILL

		}
		break;
	case KEGS_GUILD_MESSAGE_NOT::MT_KICK_MEMBER:			//	탈퇴
		{
			if ( IsMyUID( kEvent.m_iUnitUID ) == true )
			{
				SafeDeleteGuildInfo();

				if ( g_pData->GetUIManager() != NULL &&
					g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_COMMUNITY ) == true )
				{
					g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_COMMUNITY, false );		
					
				}

				if ( m_pUIGuild->GetShowGuildInfo() == true )
					m_pUIGuild->SetShowGuildInfo( false );

				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4530, "L", kEvent.m_wstrNickName ) ), g_pMain->GetNowState() );

#ifdef FIX_SEND_LIST_AT_LEAVE_GUILD	// 적용날짜: 2013-03-28
				/// 이미 탈퇴된 상태에서 존재하지 않는 길드원 리스트 ACK를 요청하지 말고, 클라 내에서 지워버리자
				if ( NULL != g_pData && NULL != g_pData->GetMessenger() )
				{
					g_pData->GetMessenger()->RemoveGuildListUI();
				}
#endif // FIX_SEND_LIST_AT_LEAVE_GUILD
			}
			else
			{
				UpdateNowNumOfGuildMember( kEvent.m_iUnitUID, false );
			}

			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4530, "L", kEvent.m_wstrNickName ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		}
		break;

	case KEGS_GUILD_MESSAGE_NOT::MT_KICK_BY_MASTER:			// 강퇴 
		{
			if ( IsMyUID( kEvent.m_iUnitUID ) == true )
			{
				SafeDeleteGuildInfo();

				if ( g_pData->GetUIManager() != NULL &&
					g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_COMMUNITY ) == true )
				{
					g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_COMMUNITY, false );
				}

				if ( m_pUIGuild->GetShowGuildInfo() == true )
					m_pUIGuild->SetShowGuildInfo( false );

				m_DlgKickByMaster = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4587 ), g_pMain->GetNowState() );

#ifdef FIX_SEND_LIST_AT_LEAVE_GUILD	// 적용날짜: 2013-03-28
				/// 이미 탈퇴된 상태에서 존재하지 않는 길드원 리스트 ACK를 요청하지 말고, 클라 내에서 지워버리자
				if ( NULL != g_pData && NULL != g_pData->GetMessenger() )
				{
					g_pData->GetMessenger()->RemoveGuildListUI();
				}
#endif // FIX_SEND_LIST_AT_LEAVE_GUILD
			}
			else
			{
				UpdateNowNumOfGuildMember( kEvent.m_iUnitUID, false );
			}

			// (일부러 탈퇴 하셨습니다 라고 찍어줌)
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4530, "L", kEvent.m_wstrNickName ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		}
		break;
	case KEGS_GUILD_MESSAGE_NOT::MT_CONNECT_MEMBER:			//	길드원 로그인
		{
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4532, "L", kEvent.m_wstrNickName ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		}
		break;
	case KEGS_GUILD_MESSAGE_NOT::MT_DISCONNECT_MEMBER:		//	길드원 로그아웃
		{
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4533, "L", kEvent.m_wstrNickName ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		}
		break;

	case KEGS_GUILD_MESSAGE_NOT::MT_GUILD_MESSAGE:
		{
			wstring wstrColor = L"#C96ff00";									// 150, 255, 0
			D3DXCOLOR coTextColor = D3DXCOLOR(0.5882f, 1, 0, 1);				// 150, 255, 0

			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4631, "L", kEvent.m_wstrMessage ) ), KEGS_CHAT_REQ::CPT_GUILD, coTextColor, wstrColor );
		}
		break;

	case KEGS_GUILD_MESSAGE_NOT::MT_GUILD_EXP:
		{
			if(g_pData->GetGuildManager()->GetMyGuildInfo() == NULL)
				return true;

			//	길드 경험치 갱신
			g_pData->GetGuildManager()->GetMyGuildInfo()->m_uiGuildEXP = kEvent.m_iMessageData;

			// 본인에 의해서 길드 경험치가 올라 갔으면
			if ( g_pData->GetMyUser() != NULL &&
				 g_pData->GetMyUser()->GetSelectUnit() != NULL &&
				 g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_iUnitUID )
			{
				g_pMain->GetInformerManager()->GetGuildInformer()->SetIncreaseGuildExpByMe( true );
				g_pMain->GetInformerManager()->GetGuildInformer()->SetNeedCheckInfo( true );
			}

			if ( GetShowUIGuildInfo() == true )
			{
				GetUIGuild()->UpdateGuildLevelBar( kEvent.m_iMessageData - GetTotalGuildExp( GetGuildInfo()->m_byGuildLevel ) , GetNextLevelNeedExp( GetGuildInfo()->m_byGuildLevel ) );
			}
		}
		break;

	case KEGS_GUILD_MESSAGE_NOT::MT_GUILD_LEVEL_UP:
		{
			//	길드 레벨 갱신
			g_pData->GetGuildManager()->GetMyGuildInfo()->m_byGuildLevel = static_cast<BYTE>( kEvent.m_iMessageData );


			//	채팅 처리
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4727, "L", kEvent.m_wstrNickName ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

#ifdef GUILD_SKILL
			// oasis907 : 김상윤 [2009.12.8] // kEvent.m_iMessageData2 새로운 GuildSPoint
			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
			int iDifference = kEvent.m_iMessageData2 - (pUnit->GetUnitData().m_iGuildSPoint);

			pUnit->AccessUnitData().m_iGuildSPoint = kEvent.m_iMessageData2;

			iDifference = 1; // 임시
			// oasis907 : 김상윤 [2009.12.3] // 길드 스킬 포인트 상승 알림
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4797, "i", iDifference)), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			//g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4797, "i", kEvent.m_iMessageData2)), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

			// oasis907 : 김상윤 [2009.12.8] // 길드 스킬 트리창 업데이트
			if( NULL != g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() )
			{
				g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->UpdateUI(true, true, true); // oasis
			}
#endif GUILD_SKILL

			if ( GetShowUIGuildInfo() == true )
			{
				GetUIGuild()->SetGuildInfoTab( CX2UIGuild::GIT_GUILD_INFO );
			}
			// 업데이트

		}
		break;


	case KEGS_GUILD_MESSAGE_NOT::MT_EXPAND_MAX_MEMBER:
		{
			//	채팅 처리
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4728, "i", kEvent.m_iMessageData) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		}
		break;

#ifdef GUILD_SKILL	
	case KEGS_GUILD_MESSAGE_NOT::MT_INSERT_GUILD_CSPOINT:
		{
			// oasis907 : 김상윤 [2009.12.2] //
			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

			int iDifference = kEvent.m_iMessageData - (pUnit->GetUnitData().m_iGuildCSPoint);

			pUnit->AccessUnitData().m_iGuildCSPoint = kEvent.m_iMessageData; // 현재 CSP 보유량
			pUnit->AccessUnitData().m_iMaxGuildCSPoint = kEvent.m_iMessageData2; // 그노시스 대축복 사용 CSP 증가량
			pUnit->AccessUnitData().m_wstrGuildCSPointEndDate = kEvent.m_wstrMessage; // CSP 만료일

			// oasis907 : 김상윤 [2009.12.3] // 그노시스 대축복 사용 CSP 증가 알림
	
			iDifference = 5; // 임시
			//g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4799, "i", kEvent.m_iMessageData2) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4799, "i", iDifference) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );


			// oasis907 : 김상윤 [2009.12.3] // 그노시스 대축복 남은 기간 표시
			wstring wstr = L"";
			wstr += GetExpirationDateDesc( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrGuildCSPointEndDate, g_pData->GetServerCurrentTime() );
	
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4800, "L", wstr ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );


			//{{ oasis907 : 김상윤 [2009.11.19] // 길드마스터의 경우 메시지 박스 생성
			if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade == CX2GuildManager::GUG_MASTER)
			{
				//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(300, 250), GET_REPLACED_STRING( ( STR_ID_4799, "i", kEvent.m_iMessageData2) ), g_pMain->GetNowState() );
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(300, 250), GET_REPLACED_STRING( ( STR_ID_4799, "i", iDifference) ), g_pMain->GetNowState() );
				return true;
			}
				
			// oasis907 : 김상윤 [2009.12.8] // 길드 스킬 트리창 업데이트
			if( NULL != g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() )
			{
				g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->UpdateUI(true, true, true); // oasis
			}

		}
		break;

#endif GUILD_SKILL

#ifdef GUILD_BOARD
	case KEGS_GUILD_MESSAGE_NOT::MT_GUILD_AD_PERIOD_INFO:
		{
			// oasis907 : 김상윤 [2010.2.2] // 
/*
			wstring tempDesc = L"";

			wstringstream wstrstm;

			wstrstm.str( L"" );

			CTime	cTime;
			KncUtil::ConvertStringToCTime( kEvent.m_wstrMessage, cTime );

			__time64_t t0 = g_pData->GetServerCurrentTime();

			double fDiffTime = difftime( KncUtil::TimeToInt( cTime ), (time_t)t0 );

			int remainDay =  (int)(fDiffTime / ( 60 * 60 * 24 ) );
			if ( remainDay > 0 )
				wstrstm << remainDay << GET_STRING( STR_ID_14 ) << L" ";

			int remainHour = ((time_t)fDiffTime % ( 60 * 60 * 24 )) / (60 * 60);
			if ( remainHour < 0 )
				remainHour = 0;

			wstrstm << remainHour << GET_STRING( STR_ID_310 ) << L" ";
			
			tempDesc += wstrstm.str().c_str();
*/
			wstring wstr = L"";

			wstr += GetExpirationDateDesc( kEvent.m_wstrMessage, g_pData->GetServerCurrentTime(), false);

			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4910, "L", wstr )), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		}
		break;
	case KEGS_GUILD_MESSAGE_NOT::MT_REG_GUILD_AD:
		{
			wstring wstr = L"";

			wstr += GetExpirationDateDesc( kEvent.m_wstrMessage, g_pData->GetServerCurrentTime(), false);

			// 길드 광고가 등록 되었습니다. (광고 유지 기간: X)
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4885, "L", wstr)), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		}
		break;
#endif GUILD_BOARD
	}

#ifdef FIX_SEND_LIST_AT_LEAVE_GUILD	// 적용날짜: 2013-03-28
	/// 내 유닛이 아니거나, 내 유닛인데 탈퇴된 상황이 아니라면 UI 갱신 시켜주자
	if ( false == IsMyUID( kEvent.m_iUnitUID ) ||
		 ( true == IsMyUID( kEvent.m_iUnitUID ) && KEGS_GUILD_MESSAGE_NOT::MT_KICK_MEMBER != kEvent.m_cMessageType && KEGS_GUILD_MESSAGE_NOT::MT_KICK_BY_MASTER != kEvent.m_cMessageType ) )
#endif // FIX_SEND_LIST_AT_LEAVE_GUILD
		UpdateGuildTabInCommunityUI();	// 고려 해봐야 할듯

	return true;
}

bool	CX2GuildManager::Handler_EGS_INVITE_GUILD_REQ( UidType unitUID ) const
{
	if ( unitUID > 0 )
	{
		if ( m_pMyGuildInfo == NULL )
			return false;

		// 길드 최대 인원 초과
		if ( m_pMyGuildInfo->m_usNowNumOfGuildMember >= _CONST_GUILD_MANAGER_::g_usMaxNumOfGuildMembers )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4539 ) , g_pMain->GetNowState() );
			return true;
		}
		
		// 길드 설정 인원 초과
		if ( m_pMyGuildInfo->m_usNowNumOfGuildMember >= m_pMyGuildInfo->m_usMaxNumOfGuildMember )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4538 ) , g_pMain->GetNowState() );
			return true;
		}

		// 이미 가입된 유저나, 존재하지 않는 유저 초대는 서버에서 처리

		KEGS_INVITE_GUILD_REQ kPacket;
		kPacket.m_iUnitUID = unitUID;

		g_pData->GetServerProtocol()->SendPacket( EGS_INVITE_GUILD_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_INVITE_GUILD_ACK );

		return true;
	}

	return false;
}

bool	CX2GuildManager::Handler_EGS_INVITE_GUILD_REQ( const wstring& wstrNickName ) const
{
	if ( wstrNickName.size() > 0 )
	{
		// 길드 최대 인원 초과
		if ( m_pMyGuildInfo->m_usNowNumOfGuildMember >= _CONST_GUILD_MANAGER_::g_usMaxNumOfGuildMembers )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4539 ) , g_pMain->GetNowState() );
			return true;
		}

		// 길드 설정 인원 초과
		if ( m_pMyGuildInfo->m_usNowNumOfGuildMember >= m_pMyGuildInfo->m_usMaxNumOfGuildMember )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4538 ) , g_pMain->GetNowState() );
			return true;
		}

		// 이미 가입된 유저나, 존재하지 않는 유저 초대는 서버에서 처리

		KEGS_INVITE_GUILD_REQ kPacket;

		kPacket.m_wstrNickName = wstrNickName;

		g_pData->GetServerProtocol()->SendPacket( EGS_INVITE_GUILD_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_INVITE_GUILD_ACK );

		return true;
	}

	return false;
}

bool	CX2GuildManager::Handler_EGS_INVITE_GUILD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) const
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INVITE_GUILD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_INVITE_GUILD_ACK ) == true )
	{
#ifdef SERV_INTEGRATION
		//{{ oasis907 : 김상윤 [2010.5.19] // 대전 던전 서버군 통합
		if ( kEvent.m_iOK == NetError::ERR_SEARCH_UNIT_03 || kEvent.m_iOK == NetError::ERR_SEARCH_UNIT_04)
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
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//	다이얼로그 닫기
			m_pUIGuild->SetShowInviteGuildMemberDlg( false );
			// oasis907 : 김상윤 [2010.2.2] //  길드 초대시 마스터에게도 다이얼로그로 알림
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4909, "L", kEvent.m_wstrReceiverNickName) ), g_pMain->GetNowState() );
			return true;
		}
	}

	return false;
}

bool	CX2GuildManager::Handler_EGS_INVITE_GUILD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INVITE_GUILD_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

#ifdef GUILD_BOARD
	//{{ oasis907 : 김상윤 [2010.1.22] // 길드 게시판의 자동 가입(수락 의사 묻지 않고)
	if( kEvent.m_bAutoJoinGuild == true )
	{
		Handler_EGS_INVITE_GUILD_REPLY_REQ( kEvent.m_kGuildInfo.m_iGuildUID, NetError::NET_OK );
		return true;
	}
	//}} oasis907 : 김상윤 [2010.1.22] // 
#endif GUILD_BOARD


	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_DUNGEON_RESULT:
		{

			// oasis907 : 김상윤 [2010.2.1] // 
			KEGS_INVITE_GUILD_NOT* pEvent = new KEGS_INVITE_GUILD_NOT;
			*pEvent = kEvent;
			g_pMain->AddReservedServerPacket( EGS_INVITE_GUILD_NOT, pEvent );
			return true;
			/*
			// EGS_INVITE_GUILD_REPLY_REQ 던전 중
			m_wstrSenderNickName = kEvent.m_wstrSenderNickName.c_str();
			Handler_EGS_INVITE_GUILD_REPLY_REQ( kEvent.m_kGuildInfo.m_iGuildUID, static_cast< int >( NetError::ERR_GUILD_17 ) );
			
			return true;
			*/
		}
		break;

	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_PVP_RESULT:
		{
			// oasis907 : 김상윤 [2010.2.1] // 
			KEGS_INVITE_GUILD_NOT* pEvent = new KEGS_INVITE_GUILD_NOT;
			*pEvent = kEvent;
			g_pMain->AddReservedServerPacket( EGS_INVITE_GUILD_NOT, pEvent );
			return true;
			/*
			// EGS_INVITE_GUILD_REPLY_REQ 대전 중
			// 위의 것과 구분 지어 줘야 할듯...
			m_wstrSenderNickName = kEvent.m_wstrSenderNickName.c_str();
			Handler_EGS_INVITE_GUILD_REPLY_REQ( kEvent.m_kGuildInfo.m_iGuildUID, static_cast< int >( NetError::ERR_GUILD_17 ) );

			return true;
			*/
		}
		break;

	default:
		break;
	}

	// 현재 초대 중인 길드와 같은 길드에서 초대 했으면
	if ( m_setGuildUIDForInvitation.find( kEvent.m_kGuildInfo.m_iGuildUID ) != m_setGuildUIDForInvitation.end() )
		return true;
	
    // 현재 초대 중인 길드와 다른 길드라면
	CKTDGUIDialogType dlgInvitation = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
		GET_REPLACED_STRING( ( STR_ID_4534, "L", kEvent.m_kGuildInfo.m_wstrGuildName ) ), 
		GUM_GUILD_JOIN_OK, g_pMain->GetNowState(), GUM_GUILD_JOIN_CANCEL );


	m_setGuildUIDForInvitation.insert( kEvent.m_kGuildInfo.m_iGuildUID );
	m_mapGuildInvitationDlg.insert( std::make_pair( dlgInvitation, kEvent.m_kGuildInfo.m_iGuildUID ) );

	

	return true;
}







bool	CX2GuildManager::Handler_RESERVED_EGS_INVITE_GUILD_NOT( KEGS_INVITE_GUILD_NOT kEvent)
{
	// 현재 초대 중인 길드와 같은 길드에서 초대 했으면
	if ( m_setGuildUIDForInvitation.find( kEvent.m_kGuildInfo.m_iGuildUID ) != m_setGuildUIDForInvitation.end() )
		return true;

	// 현재 초대 중인 길드와 다른 길드라면
	CKTDGUIDialogType dlgInvitation = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
		GET_REPLACED_STRING( ( STR_ID_4534, "L", kEvent.m_kGuildInfo.m_wstrGuildName ) ), 
		GUM_GUILD_JOIN_OK, g_pMain->GetNowState(), GUM_GUILD_JOIN_CANCEL );


	m_setGuildUIDForInvitation.insert( kEvent.m_kGuildInfo.m_iGuildUID );
	m_mapGuildInvitationDlg.insert( std::make_pair( dlgInvitation, kEvent.m_kGuildInfo.m_iGuildUID ) );
	return true;
}










bool CX2GuildManager::Handler_EGS_INVITE_GUILD_REPLY_REQ( int iGuildUID, int iReasonForDeny )
{
	if ( g_pData->GetMyUser() != NULL && 
		 g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		KEGS_INVITE_GUILD_REPLY_REQ kPacket;

		kPacket.m_iGuildUID				= iGuildUID;
		kPacket.m_iReason				= iReasonForDeny;
		kPacket.m_wstrSenderNickName	= m_wstrSenderNickName; // 초대를 한 유저의 닉네임

		g_pData->GetServerProtocol()->SendPacket( EGS_INVITE_GUILD_REPLY_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_INVITE_GUILD_REPLY_ACK );

		return true;
	}

	ASSERT( !"Wrong path" );
	return false;
}

bool CX2GuildManager::Handler_EGS_INVITE_GUILD_REPLY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) const
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INVITE_GUILD_REPLY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_INVITE_GUILD_REPLY_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}

bool CX2GuildManager::Handler_EGS_INVITE_GUILD_REPLY_DENY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INVITE_GUILD_REPLY_DENY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	// 이곳에서 초대될 유저가 던전 플레이 or 대전 플레이 중이라는 메시지 띄우기
	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_REPLACED_STRING( ( STR_ID_4610, "L", kEvent.m_wstrNickName ) ), g_pMain->GetNowState() );

	return true;
}

bool CX2GuildManager::Handler_EGS_KICK_GUILD_MEMBER_REQ( UidType iUnitUID ) const
{
	KEGS_KICK_GUILD_MEMBER_REQ kPacket;

	kPacket.m_iUnitUID = iUnitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_KICK_GUILD_MEMBER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_KICK_GUILD_MEMBER_ACK );

	return true;
}

bool CX2GuildManager::Handler_EGS_KICK_GUILD_MEMBER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_KICK_GUILD_MEMBER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_KICK_GUILD_MEMBER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			if ( kEvent.m_bKickBySysop == true ) // 누군가를 강퇴 시켰을 때
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_4589 ), g_pMain->GetNowState() );
			}

			return true;
		}
	}

	return false;
}

bool	CX2GuildManager::Handler_EGS_CHANGE_GUILD_MESSAGE_REQ( wstring &wstrMessage ) const
{
	KEGS_CHANGE_GUILD_MESSAGE_REQ kPacket;

	kPacket.m_wstrMessage		= wstrMessage;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_GUILD_MESSAGE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_GUILD_MESSAGE_ACK );

	return true;
}

bool CX2GuildManager::Handler_EGS_CHANGE_GUILD_MESSAGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_GUILD_MESSAGE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_GUILD_MESSAGE_ACK ) == true )
	{
		if(g_pMain->IsValidPacket( kEvent.m_iOK ))
		{
			m_pDlgChangeGuildMessage = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_4583 ), g_pMain->GetNowState(), GUM_GUILD_INFO_CHANGE_MESSAGE_FINAL_OK );
			GetUIGuild()->SetShowChangeGuildMessageDlg( false );
			return true;
		}
	}

	return false;
}

bool CX2GuildManager::Handler_EGS_DISBAND_GUILD_REQ() const
{
	g_pData->GetServerProtocol()->SendID( EGS_DISBAND_GUILD_REQ );
	g_pMain->AddServerPacket( EGS_DISBAND_GUILD_ACK, 10.f );

	return true;
}

bool CX2GuildManager::Handler_EGS_DISBAND_GUILD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DISBAND_GUILD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_DISBAND_GUILD_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_4582 ), g_pMain->GetNowState() );

			SafeDeleteGuildInfo();

			if ( g_pData->GetUIManager() != NULL &&
				 g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_COMMUNITY ) == true )
			{
				g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_COMMUNITY, false );
			}

			if ( m_pUIGuild->GetShowGuildInfo() == true )
				m_pUIGuild->SetShowGuildInfo( false );

			return true;
		}
	}

	return false;
}

bool CX2GuildManager::Handler_EGS_GET_GUILD_USER_LIST_REQ()
{
	KEGS_GET_GUILD_USER_LIST_REQ kPacket;

	kPacket.m_uiViewPage			= m_pUIGuild->GetNowMemberInfoPage();
	kPacket.m_cGuildUserListType	= CX2Community::GVI_GUILD_MEMBER_INFO;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_GUILD_USER_LIST_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_GET_GUILD_USER_LIST_ACK, 60.f ); 

	return true;	

}

bool	CX2GuildManager::Handler_EGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ( wstring &wstrMessage, UidType iUid ) const
{
	KEGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ kPacket;

	if( iUid == 0 )
	{
		kPacket.m_iUnitUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	}
	else
	{
		kPacket.m_iUnitUID			= iUid;
	}

	kPacket.m_wstrMessage		= wstrMessage;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK );

	return true;
}

bool	CX2GuildManager::Handler_EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK ) == true )
	{
		if(g_pMain->IsValidPacket( kEvent.m_iOK ))
		{	
			if( kEvent.m_bDelete == false ) //	삭제로 인한 패킷이 아닌 경우에만 MsgBox 띄우기
			{
				CKTDGUIDialogType pDialog = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_4592 ), g_pMain->GetNowState() );

				g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( pDialog, true );
			}

			GetUIGuild()->SetShowChangeGuildMemberMessageDlg( false );
			return Handler_EGS_GET_GUILD_USER_LIST_REQ();
		}
	}

	return false;
}

bool	CX2GuildManager::Handler_EGS_JOIN_GUILD_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_GUILD_SUCCESS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_setUidOfGuildMembers.clear();
	m_setUidOfGuildMembers.insert( kEvent.m_vecMemberUnitUID.begin(), kEvent.m_vecMemberUnitUID.end() );

	//	길드 정보 갱신
	SetGuildInfo( kEvent );

	//	UnitData에 길드명 추가
	{
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_wstrGuildName = kEvent.m_kGuildInfo.m_wstrGuildName.c_str();
	}

	// 길드탭이 열렸을 경우에
	if ( g_pData->GetMessenger() != NULL &&
		g_pData->GetMessenger()->GetOpen() == true &&
		g_pData->GetMessenger()->GetOpenTab() == CX2Community::XMUT_GUILD )
	{
		g_pData->GetMessenger()->OnGuildTabUIButton();
	}
#ifdef GUILD_SKILL
	// oasis907 : 김상윤 [2009.12.4] // 길드 가입시
	CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
	pUnit->AccessUnitData().m_UserSkillTree.SetGuildClass(0);
	pUnit->AccessUnitData().m_UserSkillTree.SetAcquiredGuildSkill( kEvent.m_kGuildSkillInfo.m_vecGuildSkill );

	// oasis907 : 김상윤 [2009.11.27] // 길드 스킬 포인트, 캐시 스킬 포인트 SET
	pUnit->AccessUnitData().m_iGuildSPoint = kEvent.m_kGuildSkillInfo.m_iGuildSPoint;
	pUnit->AccessUnitData().m_iGuildCSPoint = kEvent.m_kGuildSkillInfo.m_iGuildCSPoint;
	pUnit->AccessUnitData().m_iMaxGuildCSPoint = kEvent.m_kGuildSkillInfo.m_iMaxGuildCSPoint;
	pUnit->AccessUnitData().m_wstrGuildCSPointEndDate = kEvent.m_kGuildSkillInfo.m_wstrGuildCSPointEndDate;

	// oasis907 : 김상윤 [2009.12.3] // 그노시스 대축복 남은 기간 표시
	if(kEvent.m_kGuildSkillInfo.m_iMaxGuildCSPoint > 0 )
	{
		wstring wstr = L"";
		wstr += GetExpirationDateDesc( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrGuildCSPointEndDate, g_pData->GetServerCurrentTime() );

		g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4800, "L", wstr ) ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

	}
#endif GUILD_SKILL



	
	//	환영 팝업 추가
	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(200, 300), GET_REPLACED_STRING( ( STR_ID_4535, "L", kEvent.m_kGuildInfo.m_wstrGuildName ) ), g_pMain->GetNowState() );

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Guild_Welcome.ogg", false, false );

	return true;
}

bool CX2GuildManager::CanDestroyGuild() const
{
	const USHORT NUM_OF_GUILD_MEMBER_THAT_ALLOW_DESTROY_GUILD = 1;
	if ( m_pMyGuildInfo == NULL )
		return false;

	if ( m_pMyGuildInfo->m_usNowNumOfGuildMember > NUM_OF_GUILD_MEMBER_THAT_ALLOW_DESTROY_GUILD )
		return false;

	return true;
}

bool CX2GuildManager::CanInviteMember() const
{
	if ( g_pData->GetMyUser() == NULL ||
		g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return false;

	switch ( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade )
	{
	case GUG_MASTER:
	case GUG_SYSOP:
	case GUG_OLD_USER:
		return true;
		break;

	default:
		break;
	}

	return false;
}

void CX2GuildManager::UpdateGuildTabInCommunityUI() const
{
	// 길드탭이 열렸을 경우에
	if ( g_pData->GetMessenger() != NULL &&
		g_pData->GetMessenger()->GetOpen() == true &&
		g_pData->GetMessenger()->GetOpenTab() == CX2Community::XMUT_GUILD )
	{
		g_pData->GetMessenger()->Handler_EGS_GET_GUILD_USER_LIST_REQ();
	}
}

void CX2GuildManager::UpdateNowNumOfGuildMember( UidType iUnitUID, bool bIsAdd )
{
	if ( bIsAdd == true )
	{
		m_setUidOfGuildMembers.insert( iUnitUID );
	}
	else
	{
		if ( m_setUidOfGuildMembers.size() > 0 )
			m_setUidOfGuildMembers.erase( iUnitUID );
	}

	if ( m_pMyGuildInfo != NULL )
	{
		m_pMyGuildInfo->m_usNowNumOfGuildMember = m_setUidOfGuildMembers.size();

		BYTE byMaxNumOfGuildAdmin = m_pMyGuildInfo->m_byMaxNumOfGuildAdmin;
		m_pMyGuildInfo->UpdateMaxNumOfGuildAdmin();

		if ( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade == GUG_MASTER 
			 && byMaxNumOfGuildAdmin != m_pMyGuildInfo->m_byMaxNumOfGuildAdmin )
		{
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4618, "i", static_cast< int >( m_pMyGuildInfo->m_byMaxNumOfGuildAdmin ) ) ),
				KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		}
	}
	
}

bool CX2GuildManager::IsMyUID( UidType iMyUID ) const
{
	if ( g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit()->GetUID() == iMyUID )
		return true;
	else
		return false;
}
void CX2GuildManager::SafeDeleteGuildInfo()
{
	m_setUidOfGuildMembers.clear();
	SAFE_DELETE( m_pMyGuildInfo );

	//	길드명 삭제, 명예 포인트 삭제
	if( g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_wstrGuildName = L"";
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_iGuildHonorPoint = 0;
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_byMemberShipGrade = CX2GuildManager::GUG_INVALID;
	}
}

D3DXCOLOR CX2GuildManager::GetGradeColor( GUILD_USER_GRADE eGrade ) const
{
	D3DXCOLOR		color;
	const FLOAT		MAX_COLOR_VALUE = 255;
	color.a = 1.0f; 

	switch ( eGrade )
	{
	case GUG_MASTER:		// 길드 마스터
		{
			color.r	= 182	/ MAX_COLOR_VALUE;
			color.g = 46	/ MAX_COLOR_VALUE;
			color.b = 184	/ MAX_COLOR_VALUE;
		}
		break;

	case GUG_SYSOP:			// 관리자
		{
			color.r	= 255	/ MAX_COLOR_VALUE;
			color.g = 96	/ MAX_COLOR_VALUE;
			color.b = 0		/ MAX_COLOR_VALUE;
		}
		break;

	case GUG_OLD_USER:		// 원로 길드원
		{
			color.r	= 83	/ MAX_COLOR_VALUE;
			color.g = 180	/ MAX_COLOR_VALUE;
			color.b = 18	/ MAX_COLOR_VALUE;
		}
		break;

	case GUG_VIP_USER:		// 우수길드원
		{
			color.r	= 0		/ MAX_COLOR_VALUE;
			color.g = 173	/ MAX_COLOR_VALUE;
			color.b = 239	/ MAX_COLOR_VALUE;
		}
		break;

	case GUG_NORMAL_USER:	// 일반길드원
		{
			color.r	= 70	/ MAX_COLOR_VALUE;
			color.g = 70	/ MAX_COLOR_VALUE;
			color.b = 70	/ MAX_COLOR_VALUE;
		}
		break;

	default:
		{
			color.r = 0.0f;
			color.g = 0.0f;
			color.b = 0.0f;
		}
		break;			
	}

	return color;
}

wstring		CX2GuildManager::GetDisplayMyGuildName()
{
	wstring retGuildName = GetMyGuildName();

	if( retGuildName != L"" )
	{
		retGuildName = GET_REPLACED_STRING( ( STR_ID_4615, "L", retGuildName ) );
	}

	return retGuildName;
}

//	길드명 사이에 []를 넣어준다
wstring		CX2GuildManager::ConvertDisplayGuildName( const wstring &wstrGuildName )
{
	wstring retGuildName;

	if( wstrGuildName != L"" )
	{
		retGuildName = GET_REPLACED_STRING( ( STR_ID_4615, "L", wstrGuildName ) );
	}

	return retGuildName;
}

bool		CX2GuildManager::OpenScriptFile( const wstring wstrFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pGuildManager", this );

	if( true == wstrFileName.empty() )
		return false;

	return g_pKTDXApp->LoadLuaTinker( wstrFileName.c_str() );

}

void		CX2GuildManager::SetLimitGuildLevel_LUA( BYTE byLimitGuildLevel )
{
	m_byLimitGuildLevel = byLimitGuildLevel;
}

void		CX2GuildManager::AddGuildEXP_LUA( BYTE byGuildLevel, int iNeedEXP, int iTotalEXP )
{
	GuildEXP guildExp;

	guildExp.m_iNeedEXP		= iNeedEXP;
	guildExp.m_iTotalEXP	= iTotalEXP;

	m_mapGuildExpTable.insert( std::make_pair( byGuildLevel, guildExp ) );
}

const CX2GuildManager::GuildEXP& CX2GuildManager::GetGuildExpData( BYTE byNowGuildLevel ) const
{
	std::map< BYTE, GuildEXP >::const_iterator mItr = m_mapGuildExpTable.find( byNowGuildLevel );

	if ( mItr != m_mapGuildExpTable.end() )
		return mItr->second;
	else
	{
		ASSERT( !"Wrong Path" );

		mItr = m_mapGuildExpTable.begin();
		return mItr->second;
	}
}

bool CX2GuildManager::GetShowUIGuildInfo() const
{
	if ( m_pUIGuild == NULL || m_pUIGuild->GetGuildInfoControl() == NULL )
		return false;

	if ( m_pUIGuild->GetShowGuildInfo() == false )
		return false;

	if ( m_pUIGuild->GetGuildInfoTabType() != CX2UIGuild::GIT_GUILD_INFO )
		return false;

	return true;
}

UINT CX2GuildManager::GetNowGuildExp( BYTE byNowGuildLevel ) const
{
	if ( byNowGuildLevel < GetLimitGuildLevel() )
	{
		return g_pData->GetGuildManager()->GetMyGuildInfo()->m_uiGuildEXP;
	}
	else
	{
		//return GetNextLevelNeedExp( byNowGuildLevel - 1 );
		return 0;
	}
}

UINT CX2GuildManager::GetNextLevelNeedExp( BYTE byNowGuildLevel ) const
{
	if ( byNowGuildLevel < GetLimitGuildLevel() )
	{
		const CX2GuildManager::GuildEXP& guildExp = g_pData->GetGuildManager()->GetGuildExpData( byNowGuildLevel+1 );

		return guildExp.m_iNeedEXP;
	}
	else
	{
		return 0;
	}
}

UINT	CX2GuildManager::GetTotalGuildExp( BYTE byNowGuildLevel ) const
{
	if ( byNowGuildLevel < GetLimitGuildLevel() )
	{
		const CX2GuildManager::GuildEXP& guildExp = g_pData->GetGuildManager()->GetGuildExpData( byNowGuildLevel );

		return guildExp.m_iTotalEXP;
	}
	else
	{
		return 0;
	}
}

int	CX2GuildManager::GetNowMyGuildHonorPoint( BYTE byNowGuildLevel) const
{
	if ( byNowGuildLevel < GetLimitGuildLevel() )
	{
		return g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iGuildHonorPoint;
	}
	else
	{
		return _CONST_GUILD_MANAGER_::g_iMaxGuildHonorPoint;
	}
}
#endif	//	GUILD_MANAGEMENT
//}} 허상형 : [2009/9/15] //	길드매니저 클래스 함수 정의

