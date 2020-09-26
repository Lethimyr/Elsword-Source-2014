#include "StdAfx.h"
#include ".\x2tutorsystem.h"


CX2TutorSystem::CX2TutorSystem( CKTDXStage* pStage )
{
	m_pStage = pStage;
	m_pDLGStudentCandidateList	= NULL;
	m_bBeenUnitSelect = false;

	m_pTutorInviting = NULL;
	m_wstrInviteeName = L"";

	m_iMaxStudentCandidatePage = 1;
	m_iNowStudentCandidatePage = 1;

	m_iPickedStudentCandidateUID = -1;
	m_wstrPickedStudentCandidateName = L"";

#ifndef EFFECT_TOOL
	OpenScriptFile( L"TutorSystem.lua" );
#endif //EFFECT_TOOL
}

CX2TutorSystem::~CX2TutorSystem(void)
{
#ifndef EFFECT_TOOL
	SaveScriptFile( L"TutorSystem.lua" );
#endif //EFFECT_TOOL

	SAFE_DELETE_DIALOG( m_pDLGStudentCandidateList );


	SAFE_DELETE( m_pTutorInviting );
	for( UINT i=0; i<m_vecpTutorInvited.size(); i++ )
	{
		TutorInvited* pInvited = m_vecpTutorInvited[i];
		SAFE_DELETE( pInvited );
	}
	m_vecpTutorInvited.clear();
}


void CX2TutorSystem::OnFrameMove( double fTime, float fElapsedTime )
{
	if( NULL != m_pDLGStudentCandidateList )
	{
		if( true == m_pDLGStudentCandidateList->GetShow() &&
			true == m_pDLGStudentCandidateList->GetEnable() )
		{
			if( m_TimerRefreshCandidateList.elapsed() > 5.f )
			{
				m_TimerRefreshCandidateList.restart();
				Handler_EGS_TUTORIAL_STUDENT_LIST_REQ( m_iNowStudentCandidatePage );
			}
		}
	}


	for( UINT i=0; i<m_vecpTutorInvited.size(); i++ )
	{
		TutorInvited* pTutorInvited = m_vecpTutorInvited[i];
		pTutorInvited->OnFrameMove( fTime, fElapsedTime );

		if( pTutorInvited->m_fTimeLeftForReplyInvite <= 0.f )
		{
			SAFE_DELETE( pTutorInvited );
			m_vecpTutorInvited.erase( m_vecpTutorInvited.begin() + i );
			i--;
		}
	}


	if( NULL != m_pTutorInviting )
	{
		m_pTutorInviting->OnFrameMove( fTime, fElapsedTime );
		if( m_pTutorInviting->m_fTimeLeftForWaitingReply <= 0.f )
		{
			SAFE_DELETE( m_pTutorInviting );
		}
	}

}



bool CX2TutorSystem::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case TSUCM_VIEW_CANDIDATE_AFTER_A_WEEK:
		{
			CKTDGUICheckBox* pCheckBox_OnceAWeek = (CKTDGUICheckBox*) lParam;
			if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				wstring wstrNickName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
				TutorOption& tutorData = m_mapTutorOption[ wstrNickName ];
				tutorData.m_bViewAfterAWeek = pCheckBox_OnceAWeek->GetChecked();
			}
			
			return true;
		} break;

	case TSUCM_CLOSE_STUDENT_CANDIDATE_LIST:
		{
			OpenStudentCandidateList( false );
			return true;
		} break;

	case TSUCM_REFRESH_STUDENT_CANDIDATE_LIST:
		{
			Handler_EGS_TUTORIAL_STUDENT_LIST_REQ( m_iNowStudentCandidatePage );
			return true;
		} break;

	case TSUCM_PREV_PAGE_STUDENT_CANDIDATE_LIST:
		{
			if( m_iNowStudentCandidatePage > 1 )
			{
				Handler_EGS_TUTORIAL_STUDENT_LIST_REQ( m_iNowStudentCandidatePage-1 );
			}
			return true;
		} break;

	case TSUCM_NEXT_PAGE_STUDENT_CANDIDATE_LIST:
		{
			if( m_iNowStudentCandidatePage < m_iMaxStudentCandidatePage )
			{
				Handler_EGS_TUTORIAL_STUDENT_LIST_REQ( m_iNowStudentCandidatePage+1 );
			}
			return true;
		} break;

	case TSUCM_REQUEST_STUDENT:
		{
			if( -1 != m_iPickedStudentCandidateUID )
			{
				m_wstrInviteeName = m_wstrPickedStudentCandidateName;
				Handler_EGS_REQUEST_TUTORIAL_REQ( m_iPickedStudentCandidateUID );
			}
			return true;
		} break;

	case TSUCM_REFUSE_TUTOR_REQUEST_FOR_A_WEEK:
		{
			CKTDGUICheckBox* pCheck_RefuseForAWeek = (CKTDGUICheckBox*) lParam;
			if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				wstring wstrNickName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
				TutorOption& tutorData = m_mapTutorOption[ wstrNickName ];
				tutorData.m_bRefuseTutorForAWeek = pCheck_RefuseForAWeek->GetChecked();
			}

			return true;
		} break;

	case TSUCM_ACCEPT_TUTOR_YES:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			CKTDGUIDialogType pDialog = pButton->GetDialog();

			UidType receiverUnitUID = -1;
			if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( pDialog ) )
			{
				for( UINT i=0; i<m_vecpTutorInvited.size(); i++ )
				{
					TutorInvited* pInvited = m_vecpTutorInvited[i];
					if( pInvited->m_pDLGInvited == pDialog )
					{
						pInvited->m_fTimeLeftForReplyInvite = 0.f;
						pDialog->SetShowEnable( false, false );
						receiverUnitUID = pInvited->m_InviterUnitUID;
						break;
					}
				}
			}

			Send_Handler_EGS_REQUEST_TUTORIAL_REPLY_NOT( true, receiverUnitUID );

			return true;
		} break;
		
	case TSUCM_ACCEPT_TUTOR_NO:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			CKTDGUIDialogType pDialog = pButton->GetDialog();

			UidType receiverUnitUID = -1;
			if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( pDialog ) )
			{
				for( UINT i=0; i<m_vecpTutorInvited.size(); i++ )
				{
					TutorInvited* pInvited = m_vecpTutorInvited[i];
					if( pInvited->m_pDLGInvited == pDialog )
					{
						pInvited->m_fTimeLeftForReplyInvite = 0.f;
						pDialog->SetShowEnable( false, false );
						receiverUnitUID = pInvited->m_InviterUnitUID;
						break;
					}
				}
			}

			Send_Handler_EGS_REQUEST_TUTORIAL_REPLY_NOT( false, receiverUnitUID );

			return true;
		} break;

	case TSUCM_STUDENT_CANDIDATE_1:
		{
			if( m_vecStudentCandidate.size() > 0 )
			{
				m_iPickedStudentCandidateUID = m_vecStudentCandidate[0].m_UnitUID;
				m_wstrPickedStudentCandidateName = m_vecStudentCandidate[0].m_wstrNickName;
			}
			else
			{
				m_iPickedStudentCandidateUID = -1;
			}
			return true;

		} break;

	case TSUCM_STUDENT_CANDIDATE_2:
		{
			if( m_vecStudentCandidate.size() > 1 )
			{
				m_iPickedStudentCandidateUID = m_vecStudentCandidate[1].m_UnitUID;
				m_wstrPickedStudentCandidateName = m_vecStudentCandidate[1].m_wstrNickName;
			}
			else
			{
				m_iPickedStudentCandidateUID = -1;
			}
			return true;

		} break;

	case TSUCM_STUDENT_CANDIDATE_3:
		{
			if( m_vecStudentCandidate.size() > 2 )
			{
				m_iPickedStudentCandidateUID = m_vecStudentCandidate[2].m_UnitUID;
				m_wstrPickedStudentCandidateName = m_vecStudentCandidate[2].m_wstrNickName;
			}
			else
			{
				m_iPickedStudentCandidateUID = -1;
			}
			return true;

		} break;

	case TSUCM_STUDENT_CANDIDATE_4:
		{
			if( m_vecStudentCandidate.size() > 3 )
			{
				m_iPickedStudentCandidateUID = m_vecStudentCandidate[3].m_UnitUID;
				m_wstrPickedStudentCandidateName = m_vecStudentCandidate[3].m_wstrNickName;
			}
			else
			{
				m_iPickedStudentCandidateUID = -1;
			}
			return true;

		} break;

	case TSUCM_STUDENT_CANDIDATE_5:
		{
			if( m_vecStudentCandidate.size() > 4 )
			{
				m_iPickedStudentCandidateUID = m_vecStudentCandidate[4].m_UnitUID;
				m_wstrPickedStudentCandidateName = m_vecStudentCandidate[4].m_wstrNickName;
			}
			else
			{
				m_iPickedStudentCandidateUID = -1;
			}
			return true;

		} break;

	}

	return false;
}

bool CX2TutorSystem::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case EGS_DEL_TUTORIAL_ACK:
		{
			return Handler_EGS_DEL_TUTORIAL_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_DEL_TUTORIAL_NOT:
		{
			return Handler_EGS_DEL_TUTORIAL_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_REQUEST_TUTORIAL_ACK:
		{
			return Handler_EGS_REQUEST_TUTORIAL_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_REQUEST_TUTORIAL_NOT:
		{
			return Handler_EGS_REQUEST_TUTORIAL_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_REQUEST_TUTORIAL_REPLY_NOT:
		{
			return Handler_EGS_REQUEST_TUTORIAL_REPLY_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_TUTORIAL_STUDENT_LIST_ACK:
		{
			return Handler_EGS_TUTORIAL_STUDENT_LIST_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_TUTORIAL_UPDATE_UNIT_INFO_NOT:
		{
			return Handler_EGS_TUTORIAL_UPDATE_UNIT_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	}

	return false;
}


bool CX2TutorSystem::Handler_EGS_DEL_TUTORIAL_REQ( UidType unitUID )
{
	KEGS_DEL_TUTORIAL_REQ kPacket;
	kPacket.m_iUnitUID = unitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_DEL_TUTORIAL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_DEL_TUTORIAL_ACK );

	return true;
}

bool CX2TutorSystem::Handler_EGS_DEL_TUTORIAL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DEL_TUTORIAL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->DeleteServerPacket( EGS_DEL_TUTORIAL_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{

		}
		return true;
	}
	return false;
}


bool CX2TutorSystem::Handler_EGS_DEL_TUTORIAL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DEL_TUTORIAL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	map< UidType, TutorUnit >::iterator it;
	it = m_mapTutorUnit.find( kEvent.m_iUnitUID );
	if( it != m_mapTutorUnit.end() )
	{
		m_mapTutorUnit.erase( it );
		//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), NetError::GetErrStrF(kEvent.m_iReason), m_pStage );
#else SERV_NETERROR_STR_GET_FROM_CLIENT
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), kEvent.m_wstrReason.c_str(), m_pStage );
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		//}}
	}


	if( g_pData != NULL && g_pData->GetMessenger() != NULL )
	{
		g_pData->GetMessenger()->UpdateTutorListWindow();
	}

	return true;
}

bool CX2TutorSystem::Handler_EGS_REQUEST_TUTORIAL_REQ( UidType unitUID )
{
	KEGS_REQUEST_TUTORIAL_REQ kPacket;
	kPacket.m_iUnitUID = unitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_REQUEST_TUTORIAL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_REQUEST_TUTORIAL_ACK );

	return true;
}

bool CX2TutorSystem::Handler_EGS_REQUEST_TUTORIAL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REQUEST_TUTORIAL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->DeleteServerPacket( EGS_REQUEST_TUTORIAL_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			SAFE_DELETE( m_pTutorInviting );
			m_pTutorInviting = new TutorInviting( m_wstrInviteeName, 10.f, m_pStage );

			return true;
		}
	}

	return false;
}

bool CX2TutorSystem::Handler_EGS_REQUEST_TUTORIAL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 체험 아이디 제한 
	if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
	{
		return true;
	}

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REQUEST_TUTORIAL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	// 내 유닛을 아직 안 고른 상태면 바로 거절
	if( NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		Send_Handler_EGS_REQUEST_TUTORIAL_REPLY_NOT( false, kEvent.m_iUnitUID );
		return true;
	}


	wstring wstrNickName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
	if( true == m_mapTutorOption[ wstrNickName ].m_bRefuseTutorForAWeek )
	{
		if( false == Elapsed7DaysFromLastRefuseDate( wstrNickName ) )
		{
			Send_Handler_EGS_REQUEST_TUTORIAL_REPLY_NOT( false, kEvent.m_iUnitUID );
			return true;
		}
	}


	// 게임중에는 사제요청 팝업창 안 뜨게
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_TRAINING_GAME:
		{
			return true;
		} break;
	}


	m_mapTutorOption[ wstrNickName ].m_bRefuseTutorForAWeek = false;
	SetLastRefuseDateToday( wstrNickName );


	wstringstream wstrmText;
	wstrmText << kEvent.m_wstrNickName << L" (Lv." << (int)kEvent.m_ucLevel << L")";
	TutorInvited* pTutorInvited = new TutorInvited( kEvent.m_iUnitUID, wstrmText.str(), 10.f, TSUCM_ACCEPT_TUTOR_YES, TSUCM_ACCEPT_TUTOR_NO, m_pStage );
	m_vecpTutorInvited.push_back( pTutorInvited );
	return true;
}

bool CX2TutorSystem::Send_Handler_EGS_REQUEST_TUTORIAL_REPLY_NOT( bool bAccept, UidType receiverUnitUID )
{
	KEGS_REQUEST_TUTORIAL_REPLY_NOT kPacket;
	kPacket.m_bAccept = bAccept;
	kPacket.m_iReceiverUnitUID = receiverUnitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_REQUEST_TUTORIAL_REPLY_NOT, kPacket );

	return true;
}

bool CX2TutorSystem::Handler_EGS_REQUEST_TUTORIAL_REPLY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REQUEST_TUTORIAL_REPLY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE( m_pTutorInviting );

	if( true == kEvent.m_bAccept )
	{
		TutorUnit tutorStudent;
		tutorStudent.m_UnitUID		= kEvent.m_kTutorialUnitInfo.m_iUnitUID;
		tutorStudent.m_iLevel		= (int) kEvent.m_kTutorialUnitInfo.m_ucLevel;
		tutorStudent.m_wstrNickName = kEvent.m_kTutorialUnitInfo.m_wstrNickName;
		tutorStudent.m_bIsOnline	= kEvent.m_kTutorialUnitInfo.m_bIsOnline;

		m_mapTutorUnit[ kEvent.m_kTutorialUnitInfo.m_iUnitUID ] = tutorStudent;

		wstringstream wstrmText;
		wstrmText << GET_REPLACED_STRING( ( STR_ID_778, "L", kEvent.m_kTutorialUnitInfo.m_wstrNickName ) );
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrmText.str().c_str(), m_pStage );

		if(g_pData != NULL && g_pData->GetMessenger() != NULL)
			g_pData->GetMessenger()->UpdateTutorListWindow();
	}
	else
	{
		wstringstream wstrmText;
        wstrmText << GET_REPLACED_STRING( ( STR_ID_779, "L", kEvent.m_kTutorialUnitInfo.m_wstrNickName ) );
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrmText.str().c_str(), m_pStage );
	}

	return true;
}

bool CX2TutorSystem::Handler_EGS_TUTORIAL_STUDENT_LIST_REQ( int iPageIndex )
{
	KEGS_TUTORIAL_STUDENT_LIST_REQ kPacket;
	kPacket.m_iViewPage = iPageIndex;

	g_pData->GetServerProtocol()->SendPacket( EGS_TUTORIAL_STUDENT_LIST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_TUTORIAL_STUDENT_LIST_ACK );

	return true;
}

bool CX2TutorSystem::Handler_EGS_TUTORIAL_STUDENT_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_TUTORIAL_STUDENT_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->DeleteServerPacket( EGS_TUTORIAL_STUDENT_LIST_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			m_iMaxStudentCandidatePage = kEvent.m_iTotalPage;
			m_iNowStudentCandidatePage = kEvent.m_iViewPage;

			m_vecStudentCandidate.clear();
			for( UINT i=0; i<kEvent.m_vecTutorialStudentList.size(); i++ )
			{
				KTutorialUnitInfo& studentInfo = kEvent.m_vecTutorialStudentList[i];

				TutorUnit studentCandidate;
				studentCandidate.m_UnitUID			= studentInfo.m_iUnitUID;
				studentCandidate.m_iLevel			= (int) studentInfo.m_ucLevel;
				studentCandidate.m_wstrNickName 	= studentInfo.m_wstrNickName;
				studentCandidate.m_bIsOnline 		= studentInfo.m_bIsOnline;

				m_vecStudentCandidate.push_back( studentCandidate );
			}

			m_iPickedStudentCandidateUID = -1;
			UpdateStudentCandidateListPage();
		}
		return true;
	}

	return false;
}


bool CX2TutorSystem::Handler_EGS_TUTORIAL_UPDATE_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_TUTORIAL_UPDATE_UNIT_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	wstringstream wstrmText;
	map< UidType, TutorUnit >::iterator it = m_mapTutorUnit.find( kEvent.m_kTutorialUnitInfo.m_iUnitUID );
	if( it != m_mapTutorUnit.end() )
	{
		TutorUnit& tutorUnit = it->second;
		if( false == tutorUnit.m_bIsOnline && true == kEvent.m_kTutorialUnitInfo.m_bIsOnline )
		{
			if( g_pData->GetSelectUnitLevel() >= 20 ) // 난 스승 
			{
				wstrmText << GET_REPLACED_STRING( ( STR_ID_780, "L", kEvent.m_kTutorialUnitInfo.m_wstrNickName ) );
			}
			else // 난 제자
			{
                wstrmText << GET_REPLACED_STRING( ( STR_ID_781, "L", kEvent.m_kTutorialUnitInfo.m_wstrNickName ) );
			}
			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrmText.str().c_str(), m_pStage );

			if(g_pData != NULL && g_pData->GetMessenger() != NULL)
			{
				g_pData->GetMessenger()->AlarmReceiveMessage( wstrmText.str().c_str() );
			}
			
		}
	}
	else
	{
		if( g_pData->GetSelectUnitLevel() >= 20 ) // 난 스승 
		{
			;
		}
		else // 난 제자
		{
			if( 0 == m_mapTutorUnit.size() )
			{
				wstrmText << GET_REPLACED_STRING( ( STR_ID_782, "L", kEvent.m_kTutorialUnitInfo.m_wstrNickName ) );
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrmText.str().c_str(), m_pStage );
			}
		}
	}

	TutorUnit tutorStudent;
	tutorStudent.m_UnitUID		= kEvent.m_kTutorialUnitInfo.m_iUnitUID;
	tutorStudent.m_iLevel		= (int) kEvent.m_kTutorialUnitInfo.m_ucLevel;
	tutorStudent.m_wstrNickName = kEvent.m_kTutorialUnitInfo.m_wstrNickName;
	tutorStudent.m_bIsOnline	= kEvent.m_kTutorialUnitInfo.m_bIsOnline;

	m_mapTutorUnit[ kEvent.m_kTutorialUnitInfo.m_iUnitUID ] = tutorStudent;


	if(g_pData != NULL && g_pData->GetMessenger() != NULL)
		g_pData->GetMessenger()->UpdateTutorListWindow();

	return true;
};


bool CX2TutorSystem::OpenScriptFile( const WCHAR* pFileName ) 
{

	// TutorSystem.lua 파일을 읽어서 UTF-8 포맷이 아니면 변환한다
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

    KLuaManagerProxy luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
    if ( g_pKTDXApp->LoadAndDoMemory_LocalFile( &luaManager, pFileName ) == false )
        return false;

#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

    KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
// 	if( false == g_pKTDXApp->LoadLuaTinker( pFileName, false ) )
// 		return false;
	if( false == g_pKTDXApp->LoadAndDoMemory_LocalFile( &luaManager, pFileName ) )
		return false;

	if ( false == luaManager.ExportFunctionsToGlobalEnv() )
		return false;
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	int iUnitIndex = 0;
	while( luaManager.BeginTable( "UNIT", iUnitIndex ) == true )
	{
		wstring wstrNickName = L"";
		LUA_GET_VALUE( luaManager,	"UNIT_NAME",							wstrNickName,										L"" );

		TutorOption tutorOption;
		LUA_GET_VALUE( luaManager,	"VIEW_AFTER_A_WEEK",					tutorOption.m_bViewAfterAWeek,						false );
		LUA_GET_VALUE( luaManager,	"LAST_VIEW_YDAY_CANDIDATE_LIST",		tutorOption.m_iLastViewYDayPossibleStudentList,		-1 );


		LUA_GET_VALUE( luaManager,	"REFUSE_FOR_A_WEEK",					tutorOption.m_bRefuseTutorForAWeek,					false );
		LUA_GET_VALUE( luaManager,	"LAST_REFUSE_YDAY_TUTOR_REQUEST",		tutorOption.m_iLastRefuseYDay,						-1 );


		m_mapTutorOption[ wstrNickName ] = tutorOption;

		luaManager.EndTable(); // UNIT( i )
		iUnitIndex++;
	}


	return true;
}

bool CX2TutorSystem::SaveScriptFile( const WCHAR* pFileName )
{
	if( NULL == pFileName )
		return false;


	// 내 계정에 없는 캐릭터에 대한 옵션은 삭제하자
	map< wstring, TutorOption >	mapTempTutorOption;
	if( NULL != g_pData && 
		NULL != g_pData->GetMyUser() )
	{
		map< wstring, TutorOption >::iterator it;
		for( it = m_mapTutorOption.begin(); it != m_mapTutorOption.end() ; it++ )
		{
			TutorOption& tutorOption = it->second;
			wstring wstrNickName = it->first;

			CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByNickName( wstrNickName.c_str() );
			if( NULL != pUnit )
			{
				mapTempTutorOption[ wstrNickName ] = tutorOption;
			}
		}

		m_mapTutorOption.clear();
		m_mapTutorOption = mapTempTutorOption;
	}


	string strFileName = "";
	ConvertWCHARToChar( strFileName, pFileName );

	std::ofstream fout;
	fout.open( strFileName.c_str(), std::ios_base::binary );
	if( false == fout.is_open() )
		return false;

    fout << (UCHAR) 0xEF << (UCHAR) 0xBB << (UCHAR) 0xBF;

	int iUnitIndex = 0; 
	map< wstring, TutorOption >::iterator it;
	for( it = m_mapTutorOption.begin(); it != m_mapTutorOption.end() ; it++, iUnitIndex++ )
	{
		TutorOption& tutorData = it->second;

		fout << "UNIT" << iUnitIndex << " = " << std::endl;
		fout << "{" << std::endl;

		string strNickName = "";
		ConvertWCHARToChar( strNickName, it->first.c_str() );
		fout << "\t UNIT_NAME = \"" << strNickName << "\"," << std::endl;

		if( true == tutorData.m_bViewAfterAWeek )
		{
			fout << "\t VIEW_AFTER_A_WEEK = TRUE, " << std::endl;
		}
		else
		{
			fout << "\t VIEW_AFTER_A_WEEK = FALSE, " << std::endl;
		}

		fout << "\t LAST_VIEW_YDAY_CANDIDATE_LIST = " << tutorData.m_iLastViewYDayPossibleStudentList << ", " << std::endl;



		if( true == tutorData.m_bRefuseTutorForAWeek )
		{
			fout << "\t REFUSE_FOR_A_WEEK = TRUE, " << std::endl;
		}
		else
		{
			fout << "\t REFUSE_FOR_A_WEEK = FALSE, " << std::endl;
		}

		fout << "\t LAST_REFUSE_YDAY_TUTOR_REQUEST = " << tutorData.m_iLastRefuseYDay << ", " << std::endl;


		fout << "}" << std::endl;

		fout.flush();
	}

	fout.close();
	return true;
}

void CX2TutorSystem::SetStage( CKTDXStage* val )
{
	m_pStage = val;

	if( NULL != m_pDLGStudentCandidateList )
	{
		m_pDLGStudentCandidateList->SetStage( m_pStage );
	}

	for( UINT i=0; i<m_vecpTutorInvited.size(); i++ )
	{
		TutorInvited* pTutorInvited = m_vecpTutorInvited[i];
		if( NULL != pTutorInvited &&
			NULL != pTutorInvited->m_pDLGInvited )
		{
			pTutorInvited->m_pDLGInvited->SetStage( m_pStage );
		}
	}
}


void CX2TutorSystem::OpenStudentCandidateList( bool bOpen )
{
	if( true == bOpen )
	{
		if( NULL == m_pDLGStudentCandidateList )
		{
			m_pDLGStudentCandidateList = new CKTDGUIDialog( m_pStage, L"DLG_Student_List_Window.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGStudentCandidateList );
		}

		if( NULL != m_pDLGStudentCandidateList )
		{
			m_pDLGStudentCandidateList->SetShowEnable( true, true );
		}
	}
	else
	{
		if( NULL != m_pDLGStudentCandidateList )
		{
			m_pDLGStudentCandidateList->SetShowEnable( false, false );
		}
	}
}

void CX2TutorSystem::UpdateStudentCandidateListPage()
{
	if( NULL == m_pDLGStudentCandidateList )
		return;


	if( NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		wstring wstrNickName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
		if( wstrNickName.length() > 0 )
		{
			const TutorOption& tutorOption = m_mapTutorOption[ wstrNickName ];
			CKTDGUICheckBox* pCheck_ViewNextWeek = (CKTDGUICheckBox*) m_pDLGStudentCandidateList->GetControl( L"g_pCheckBoxClose_Window" );
			pCheck_ViewNextWeek->SetChecked( tutorOption.m_bViewAfterAWeek );
		}
	}


	CKTDGUIStatic* pStatic_PageNum = (CKTDGUIStatic*) m_pDLGStudentCandidateList->GetControl( L"g_pStaticPage_Font" );
	if( NULL != pStatic_PageNum )
	{
		wstringstream wstrmText;
		wstrmText << m_iNowStudentCandidatePage << L"/" << m_iMaxStudentCandidatePage;
		pStatic_PageNum->GetString(0)->msg = wstrmText.str();
	}


	WCHAR wszText[64] = L"";
	for( int i=0; i<5; i++ )
	{
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"g_pStaticNicName%d", i+1 );
		CKTDGUIStatic* pStatic_NickName = (CKTDGUIStatic*) m_pDLGStudentCandidateList->GetControl( wszText );
		if( NULL != pStatic_NickName )
		{
			pStatic_NickName->SetShowEnable( false, false );
		}

		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"g_pStaticLV_%d", i+1 );
		CKTDGUIStatic* pStatic_Level = (CKTDGUIStatic*) m_pDLGStudentCandidateList->GetControl( wszText );
		if( NULL != pStatic_Level )
		{
			pStatic_Level->SetShowEnable( false, false );
		}


		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"g_pStaticState%d", i+1 );
		CKTDGUIStatic* pStatic_State = (CKTDGUIStatic*) m_pDLGStudentCandidateList->GetControl( wszText );
		if( NULL != pStatic_State )
		{
			pStatic_State->SetShowEnable( false, false );
		}

		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"Student_List%d", i+1 );
		CKTDGUIRadioButton* pRadio_Candidate = (CKTDGUIRadioButton*) m_pDLGStudentCandidateList->GetControl( wszText );
		if( NULL != pRadio_Candidate )
		{
			pRadio_Candidate->SetShowEnable( false, false );
			pRadio_Candidate->SetChecked( false );
		}
	}

	for( UINT i=0; i<m_vecStudentCandidate.size() && i<5 ; i++ )
	{
		TutorUnit& studentCandidate = m_vecStudentCandidate[i];

		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"g_pStaticNicName%d", i+1 );
		CKTDGUIStatic* pStatic_NickName = (CKTDGUIStatic*) m_pDLGStudentCandidateList->GetControl( wszText );
		if( NULL != pStatic_NickName )
		{
			pStatic_NickName->SetShowEnable( true, true );
			pStatic_NickName->GetString(0)->msg = studentCandidate.m_wstrNickName;
		}

		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"g_pStaticLV_%d", i+1 );
		CKTDGUIStatic* pStatic_Level = (CKTDGUIStatic*) m_pDLGStudentCandidateList->GetControl( wszText );
		if( NULL != pStatic_Level )
		{
			pStatic_Level->SetShowEnable( true, true );
			wstringstream wstrmText;
			wstrmText << studentCandidate.m_iLevel;
			pStatic_Level->GetString(0)->msg = wstrmText.str();
		}


		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"g_pStaticState%d", i+1 );
		CKTDGUIStatic* pStatic_State = (CKTDGUIStatic*) m_pDLGStudentCandidateList->GetControl( wszText );
		if( NULL != pStatic_State )
		{
			pStatic_State->SetShowEnable( true, true );
			if( true == studentCandidate.m_bIsOnline )
			{
				pStatic_State->GetString(0)->msg = GET_STRING( STR_ID_98 );
			}
			else
			{
				pStatic_State->GetString(0)->msg = GET_STRING( STR_ID_99 );
			}
		}

		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"Student_List%d", i+1 );
		CKTDGUIRadioButton* pRadio_Candidate = (CKTDGUIRadioButton*) m_pDLGStudentCandidateList->GetControl( wszText );
		if( NULL != pRadio_Candidate )
		{
			pRadio_Candidate->SetShowEnable( true, true );
		}
	}
}


bool CX2TutorSystem::Elapsed7DaysFromLastViewDate( wstring wstrNickName )
{
	time_t nowTime = time( NULL );
#ifdef	CONVERSION_VS
    struct tm nowDate;
    struct tm* pnowDate = &nowDate;
    bool bOK = localtime_s( pnowDate, &nowTime ) == 0;
#else   CONVERSION_VS
	tm* pnowDate = localtime( &nowTime );
	bool bOK = ( pnowDate != NULL );
#endif  CONVERSION_VS

	if ( bOK == false )
		return false;

	if( m_mapTutorOption[wstrNickName].m_iLastViewYDayPossibleStudentList < 0 ||						// 저장된 last_view_date가 없거나
		pnowDate->tm_yday - m_mapTutorOption[wstrNickName].m_iLastViewYDayPossibleStudentList < 0 ||	// 해(year)가 바뀌었거나
		pnowDate->tm_yday - m_mapTutorOption[wstrNickName].m_iLastViewYDayPossibleStudentList >= 7 )	// 일주일이 지난 경우에
		return true;
	else 
		return false;
}

bool CX2TutorSystem::Elapsed1DaysFromLastViewDate( wstring wstrNickName )
{
	time_t nowTime = time( NULL );
#ifdef	CONVERSION_VS
    struct tm nowDate;
    struct tm* pnowDate = &nowDate;
    bool bOK = localtime_s( pnowDate, &nowTime ) == 0;
#else   CONVERSION_VS
	tm* pnowDate = localtime( &nowTime );
	bool bOK = ( pnowDate != NULL );
#endif  CONVERSION_VS

	if ( bOK == false )
		return false;


	if( m_mapTutorOption[wstrNickName].m_iLastViewYDayPossibleStudentList < 0 ||						// 저장된 last_view_date가 없거나
		pnowDate->tm_yday - m_mapTutorOption[wstrNickName].m_iLastViewYDayPossibleStudentList < 0 ||	// 해(year)가 바뀌었거나
		pnowDate->tm_yday - m_mapTutorOption[wstrNickName].m_iLastViewYDayPossibleStudentList >= 1 )	// 하루가 지난 경우에
		return true;
	else 
		return false;
}


bool CX2TutorSystem::Elapsed7DaysFromLastRefuseDate( wstring wstrNickName )
{
	time_t nowTime = time( NULL );
#ifdef	CONVERSION_VS
    struct tm nowDate;
    struct tm* pnowDate = &nowDate;
    bool bOK = localtime_s( pnowDate, &nowTime ) == 0;
#else   CONVERSION_VS
	tm* pnowDate = localtime( &nowTime );
	bool bOK = ( pnowDate != NULL );
#endif  CONVERSION_VS

	if ( bOK == false )
		return false;


	if( m_mapTutorOption[wstrNickName].m_iLastRefuseYDay < 0 ||						// 저장된 last_view_date가 없거나
		pnowDate->tm_yday - m_mapTutorOption[wstrNickName].m_iLastRefuseYDay < 0 ||	// 해(year)가 바뀌었거나
		pnowDate->tm_yday - m_mapTutorOption[wstrNickName].m_iLastRefuseYDay >= 7 )	// 일주일이 지난 경우에
		return true;
	else 
		return false;
}

void CX2TutorSystem::SetLastViewDateToday( wstring wstrNickName )
{
	time_t nowTime = time( NULL );
#ifdef	CONVERSION_VS
    struct tm nowDate;
    struct tm* pnowDate = &nowDate;
    bool bOK = localtime_s( pnowDate, &nowTime ) == 0;
#else   CONVERSION_VS
	tm* pnowDate = localtime( &nowTime );
	bool bOK = ( pnowDate != NULL );
#endif  CONVERSION_VS

	if ( bOK == false )
		return;


	m_mapTutorOption[wstrNickName].m_iLastViewYDayPossibleStudentList = pnowDate->tm_yday;
}


void CX2TutorSystem::SetLastRefuseDateToday( wstring wstrNickName )
{
	time_t nowTime = time( NULL );
#ifdef	CONVERSION_VS
    struct tm nowDate;
    struct tm* pnowDate = &nowDate;
    bool bOK = localtime_s( pnowDate, &nowTime ) == 0;
#else   CONVERSION_VS
	tm* pnowDate = localtime( &nowTime );
	bool bOK = ( pnowDate != NULL );
#endif  CONVERSION_VS

	if ( bOK == false )
		return;


	m_mapTutorOption[wstrNickName].m_iLastRefuseYDay = pnowDate->tm_yday;
}


void CX2TutorSystem::ResetTutorStudent( vector<KTutorialDBUnitInfo>& vecStudent )
{
	m_mapTutorUnit.clear();

	for( UINT i=0; i<vecStudent.size(); i++ )
	{
		KTutorialDBUnitInfo& dbStudentInfo = vecStudent[i];
		KTutorialUnitInfo& studentInfo = dbStudentInfo.m_kTutorialUnitInfo;

		TutorUnit tutorStudent;
		tutorStudent.m_UnitUID		= studentInfo.m_iUnitUID;
		tutorStudent.m_iLevel		= (int) studentInfo.m_ucLevel;
		tutorStudent.m_wstrNickName = studentInfo.m_wstrNickName;
		tutorStudent.m_bIsOnline	= studentInfo.m_bIsOnline;

		m_mapTutorUnit[ studentInfo.m_iUnitUID ] = tutorStudent;
	}
}

const CX2TutorSystem::TutorUnit* CX2TutorSystem::GetTutorUnit( UidType unitUID ) const
{
	map< UidType, TutorUnit >::const_iterator it;
	it = m_mapTutorUnit.find( unitUID );
	if( it == m_mapTutorUnit.end() )
		return NULL;

	return (TutorUnit*) &(it->second);
}


//////////////////////////////////////////////////////////////////////////
CX2TutorSystem::TutorInviting::TutorInviting( wstring& wstrInviteeName, float fTimeLeft, CKTDXStage* pNowStage )
{
	m_pDLGInviting = NULL;
	m_wstrInviteeName = wstrInviteeName;
	m_fTimeLeftForWaitingReply = fTimeLeft;

	m_pDLGInviting = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_783, "Li", m_wstrInviteeName, ( int )m_fTimeLeftForWaitingReply ) ), (CKTDXStage*)pNowStage );
}

CX2TutorSystem::TutorInviting::~TutorInviting()
{
	SAFE_DELETE_DIALOG( m_pDLGInviting );
}

void CX2TutorSystem::TutorInviting::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( m_fTimeLeftForWaitingReply > 0.f )
	{
		int iOldTimeLeft = (int)m_fTimeLeftForWaitingReply;
		m_fTimeLeftForWaitingReply -= fElapsedTime;

		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGInviting ) == true )
		{
			if( m_fTimeLeftForWaitingReply < 0.f )
			{
				if( NULL != m_pDLGInviting && true == m_pDLGInviting->GetShow() )
				{
					m_pDLGInviting->SetShow( false );
					m_pDLGInviting->SetEnable( false );
				}
			}
			else
			{
				if( NULL != m_pDLGInviting && 
					true == m_pDLGInviting->GetShow() &&
					iOldTimeLeft != (int)m_fTimeLeftForWaitingReply )
				{
					CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGInviting->GetControl( L"StaticMsgBoxBG" );

					if( pStatic != NULL )
					{
						pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_783, "Li", m_wstrInviteeName, ( int )m_fTimeLeftForWaitingReply ) );
					}					
				}
			}
		}	
	}
}




//////////////////////////////////////////////////////////////////////////
CX2TutorSystem::TutorInvited::TutorInvited( UidType InviterUnitUID, wstring& wstrInviterName, 
											float fTimeLeft, int YesMsgID, int NoMsgID, CKTDXStage* pNowStage )
{
	m_pDLGInvited		= NULL;
	m_InviterUnitUID	= InviterUnitUID;
	m_wstrInviterName	= wstrInviterName;
	m_fTimeLeftForReplyInvite = fTimeLeft;


	m_pDLGInvited = new CKTDGUIDialog( pNowStage, L"DLG_Student_Popup.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGInvited );


	CKTDGUIStatic* pStatic_ID = (CKTDGUIStatic*) m_pDLGInvited->GetControl( L"g_pStatic_Item_ID_1Line" );
	pStatic_ID->GetString(0)->msg = L"[";
	pStatic_ID->GetString(0)->msg += m_wstrInviterName;
	pStatic_ID->GetString(0)->msg += L"]";

}

CX2TutorSystem::TutorInvited::~TutorInvited()
{
	SAFE_DELETE_DIALOG( m_pDLGInvited );
}

void CX2TutorSystem::TutorInvited::OnFrameMove( double fTime, float fElapsedTime )
{

}
