#include "StdAfx.h"
#include ".\x2noticemanager.h"



CX2NoticeManager::CX2NoticeManager( int noticeGapWidth )
{
	m_NoticeGapWidth = noticeGapWidth;
	for(UINT i = 0; i < MAX_NOTICE_NUM; i++)
	{
		m_pDlgTakeItemBackBg[i] = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Notice_Background.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgTakeItemBackBg[i] );
		m_pDlgTakeItemBackBg[i]->SetShow(false);
		m_pDlgTakeItemBackBg[i]->SetPos(D3DXVECTOR2(0.f, 748.f-i*20.f));
		m_pDlgTakeItemBackBg[i]->AddDummyInt(i);

		m_fTakeItemNoticeTime[i] = 0.0f;	/// 초기화
	}
#ifdef SERV_SHUTDOWN_SYSTEM	// 빌드 오류로 해외팀 추가
	bShutDownNotice = false;
#endif // SERV_SHUTDOWN_SYSTEM

#ifdef ADD_HERO_MATCH_NOTICE
	m_bHeroMatchWisperNotice = false;		/// 귓속말 공지로 설정해야 할 지 선택용 변수
#endif ADD_HERO_MATCH_NOTICE
}

CX2NoticeManager::~CX2NoticeManager(void)
{
	for(UINT i = 0; i < MAX_NOTICE_NUM; i++)
	{
		SAFE_DELETE_DIALOG(m_pDlgTakeItemBackBg[i])
	}
	//SAFE_CLOSE( m_pTextureTakeItemBackBg );

	while( m_queNoticeData.empty() == false )
	{
		NoticeData* pNoticeData = m_queNoticeData.front();
		SAFE_DELETE( pNoticeData );

		m_queNoticeData.pop();
	}

	while( m_queTakeItemNoticeData.empty() == false )
	{
		NoticeData* pNoticeData = m_queTakeItemNoticeData.front();
		SAFE_DELETE( pNoticeData );

		m_queTakeItemNoticeData.pop();
	}	
}


bool CX2NoticeManager::AddNotice( KEGS_NOTIFY_MSG_NOT::NOTIFY_TYPE eNotifyType, const WCHAR* pMsg, int repeatNum /* = 1 */ )
{
	if ( pMsg == NULL )
		return false;

	NoticeData* pNoticeData = new NoticeData();	
	pNoticeData->m_Msg			= L"* ";
	pNoticeData->m_Msg			+= pMsg;
	pNoticeData->m_RepeatCount	= repeatNum;	



	// 임시로 중복된 공지가 연속해서 날아오면 무시하도록 한다
	//{
	//	std::queue< NoticeData* > queTemp = m_queNoticeData;
	//	while( false == queTemp.empty() )
	//	{
	//		NoticeData* pTempNoticeData = queTemp.front();
	//		queTemp.pop();

	//		if( 0 == pTempNoticeData->m_Msg.compare( pNoticeData->m_Msg ) )
	//		{
	//			return false;
	//		}
	//	}


	//	for( UINT i=0; i< m_TextListToDelete.size(); i++ )
	//	{
	//		CX2TextManager::CX2Text* pX2Text = m_TextListToDelete[i];
	//		if( pX2Text != NULL )
	//		{
	//			if( g_pMain->GetTextManager()->CheckText( pX2Text ) == true )
	//			{
	//				if( 0 == pNoticeData->m_Msg.compare( pX2Text->GetTextMsg() ) )
	//				{
	//					return false;
	//				}
	//			}
	//		}
	//	}

	//	for( UINT i=0; i<m_vecSpamNotice.size(); i++ )
	//	{
	//		SpamNotice& spamNotice = m_vecSpamNotice[i];
	//		if( spamNotice.m_Timer.elapsed() > 300.f )
	//		{
	//			m_vecSpamNotice.erase( m_vecSpamNotice.begin() + i );
	//			i--;
	//			continue;
	//		}

	//		if( 0 == spamNotice.m_Msg.compare( pNoticeData->m_Msg ) )
	//		{
	//			return false;
	//		}
	//	}


	//	SpamNotice spamNotice;
	//	spamNotice.m_Timer.restart();
	//	spamNotice.m_Msg = pNoticeData->m_Msg;
	//	m_vecSpamNotice.push_back( spamNotice );
	//}

	//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
	if(eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_ALL_SERVERS ||
		eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_SERVER_GROUP
#ifdef SERV_SHUTDOWN_SYSTEM
		|| eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_SHUT_DOWN_ALL_NOTIFY
#endif
		)
	//}}
	{
		m_queNoticeData.push( pNoticeData );
	}
	else if( eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_DROP_GET_ITEM ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_MAX_LEVEL_UP ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_ENCHANT_SUCCESS ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_HENIR_CLEAR ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GUILD_CREATION ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GUILD_LEVEL_UP 
#ifdef SERV_GUILD_CHANGE_NAME
			 || eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GUILD_NAME_CHANGED
#endif //CHANGE_GUILD_NAME
#ifdef SERV_RELATIONSHIP_SYSTEM
			 || eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_WEDDING_SUCCESS
#endif //SERV_RELATIONSHIP_SYSTEM
			 )
	{
		m_queTakeItemNoticeData.push( pNoticeData );
	}
	else
	{
		SAFE_DELETE(pNoticeData);
	}

#ifdef SERV_SHUTDOWN_SYSTEM
	if ( eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_SHUT_DOWN_ALL_NOTIFY )
		bShutDownNotice = true;
	else
		bShutDownNotice = false;
#endif

	return true;
}

HRESULT	CX2NoticeManager::OnFrameMove( double fTime, float fElapsedTime )
{
	// 일반공지
	for ( int i = 0; i < (int)m_TextListToDelete.size(); i++ )
	{
		CX2TextManager::CX2Text* pX2Text = m_TextListToDelete[i];
		if ( pX2Text != NULL )
		{
			if ( g_pMain->GetTextManager()->CheckText( pX2Text ) == false )
			{
				m_TextListToDelete.erase( m_TextListToDelete.begin() + i );
				i--;
			}
		}
	}

	if ( m_TextListToDelete.empty() == true )
	{
		if ( m_queNoticeData.empty() == false )
		{
			AddNoticeData();
		} 
	}
	else
	{
		if ( m_queNoticeData.empty() == false )
		{
			CX2TextManager::CX2Text* pCXText = m_TextListToDelete[ m_TextListToDelete.size() - 1 ];
			if ( pCXText != NULL )
			{
				int textWidth = pCXText->GetNowTextWidth();
				D3DXVECTOR2 textPos = pCXText->GetPos();

				if ( textPos.x + textWidth < 1024 - m_NoticeGapWidth )
				{
					AddNoticeData();
				}
			}
		}
	}
	// 아이템 획득 공지
	//시간 다된 공지 제거
	for( UINT i = 0; i < MAX_NOTICE_NUM ; i++)
	{
		m_fTakeItemNoticeTime[i] -= fElapsedTime;

		if( m_fTakeItemNoticeTime[i] < 0.f)
		{
			m_pDlgTakeItemBackBg[i]->SetShow(false);
		}
		else
		{
			m_pDlgTakeItemBackBg[i]->SetShow(true);
		}
	}

	//공지 등록
	AddTakeItemNoticeData();

	return S_OK;
}

void CX2NoticeManager::AddNoticeData()
{
	NoticeData* tempNotice = m_queNoticeData.front();

	if ( tempNotice != NULL )
	{
		tempNotice->m_RepeatCount -= 1;

		CX2TextManager::CX2Text* pCXText;
#ifdef SERV_SHUTDOWN_SYSTEM	//#CFF00FF
		if ( true == bShutDownNotice )
			pCXText = g_pMain->GetTextManager()->AddText( XUF_DODUM_20_BOLD, D3DXVECTOR2( 1024., 260 ), tempNotice->m_Msg.c_str(), D3DXCOLOR(1,0.35f,0,1), D3DXCOLOR(0,0,0,1) );
#ifdef ADD_HERO_MATCH_NOTICE		/// 귓속말 공지는 자주색 및 아래 위치에 표시
		else if ( true == GetHeroMatchWisperNotice() )
			pCXText = g_pMain->GetTextManager()->AddText( XUF_DODUM_20_BOLD, D3DXVECTOR2( 1024.f, 400.f ), tempNotice->m_Msg.c_str(), D3DXCOLOR(1.f,0.f,1.f,1.f), D3DXCOLOR(0,0,0,1) );
#endif ADD_HERO_MATCH_NOTICE
		else
			pCXText = g_pMain->GetTextManager()->AddText( XUF_DODUM_20_BOLD, D3DXVECTOR2( 1024, 260 ), tempNotice->m_Msg.c_str(), D3DXCOLOR(1,1,0,1), D3DXCOLOR(0,0,0,1) );
#else
		pCXText = g_pMain->GetTextManager()->AddText( XUF_DODUM_20_BOLD, D3DXVECTOR2( 1024, 260 ), tempNotice->m_Msg.c_str(), 
			D3DXCOLOR(1,1,0,1), D3DXCOLOR(0,0,0,1) );
#endif

		//20글자를 기준으로 10초라고 생각한다?
		float fChangeTime;
		fChangeTime = 23.0f * ( (float)pCXText->GetTextWidth( tempNotice->m_Msg.c_str() ) + 1024.0f ) / 1024.0f;

#ifdef SERV_SHUTDOWN_SYSTEM
		if ( true == bShutDownNotice )
			pCXText->Move( D3DXVECTOR2( (float)(0 - pCXText->GetTextWidth( tempNotice->m_Msg.c_str() ) ) , 260), tempNotice->m_Msg.c_str(), D3DXCOLOR(1,0.35f,0,1), D3DXCOLOR(0,0,0,1), fChangeTime, false, true );
#ifdef ADD_HERO_MATCH_NOTICE		/// 귓속말 공지는 자주색 및 아래 위치에 표시
		else if ( true == GetHeroMatchWisperNotice() )
			pCXText->Move( D3DXVECTOR2( (float)(0 - pCXText->GetTextWidth( tempNotice->m_Msg.c_str() ) ) , 400.f), tempNotice->m_Msg.c_str(), D3DXCOLOR(1.f,0.f,1.f,1.f), D3DXCOLOR(0,0,0,1), fChangeTime, false, true );
#endif ADD_HERO_MATCH_NOTICE
		else
			pCXText->Move( D3DXVECTOR2( (float)(0 - pCXText->GetTextWidth( tempNotice->m_Msg.c_str() ) ) , 260), tempNotice->m_Msg.c_str(), D3DXCOLOR(1,1,0,1), D3DXCOLOR(0,0,0,1), fChangeTime, false, true );
#else
		pCXText->Move( D3DXVECTOR2( (float)(0 - pCXText->GetTextWidth( tempNotice->m_Msg.c_str() ) ) , 260), tempNotice->m_Msg.c_str(), D3DXCOLOR(1,1,0,1), D3DXCOLOR(0,0,0,1), fChangeTime, false, true );
#endif

		m_TextListToDelete.push_back( pCXText );
		if ( tempNotice->m_RepeatCount <= 0 )
		{
			SAFE_DELETE( tempNotice );
			m_queNoticeData.pop();
		}
	}

#ifdef ADD_HERO_MATCH_NOTICE
	if( true == GetHeroMatchWisperNotice() )
		SetHeroMatchWisperNotice( false );
#endif ADD_HERO_MATCH_NOTICE
}

void CX2NoticeManager::AddTakeItemNoticeData()
{
	if( m_queTakeItemNoticeData.empty() == true )
	{
		return;
	}

	NoticeData* tempNotice = m_queTakeItemNoticeData.front();
	if ( tempNotice != NULL )
	{
		//각 알림 별 메세지의 컨트롤 얻기
		CKTDGUIStatic *pStatic[MAX_NOTICE_NUM];
		for( UINT i = 0; i < MAX_NOTICE_NUM; ++i)
		{
			if( NULL != m_pDlgTakeItemBackBg[i] )
			{
				pStatic[i] = (CKTDGUIStatic*)m_pDlgTakeItemBackBg[i]->GetControl(L"Notice_Message");
			}
		}
		
		//공지가 추가되면 기존 공지가 하나씩 위로 올라가도록 설정.
		for( UINT i = MAX_NOTICE_NUM; i > 1 ; --i)
		{
			if( NULL != pStatic[i-1] && NULL != pStatic[i-2])
			{
				pStatic[i-1]->GetString(0)->msg = pStatic[i-2]->GetString(0)->msg;
				m_fTakeItemNoticeTime[i-1] = m_fTakeItemNoticeTime[i-2];
			}
		}

		//새롭게 들어가는 공지 메세지 설정
		if(NULL != pStatic[0] && NULL != m_pDlgTakeItemBackBg[0])
		{
			pStatic[0]->GetString(0)->msg = tempNotice->m_Msg;
			m_fTakeItemNoticeTime[0] = 6.f;
			m_pDlgTakeItemBackBg[0]->SetShow(true);
		}

		tempNotice->m_RepeatCount = 0;

		//채팅 로그 추가
		D3DXCOLOR _color = D3DXCOLOR( 1,1,0,1 );
		if ( g_pChatBox != NULL )
			g_pChatBox->AddChatLog( tempNotice->m_Msg.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, _color, L"#CFFFF00" );

		//추가된 공지 제거
		if ( tempNotice->m_RepeatCount <= 0 )
		{
			SAFE_DELETE( tempNotice );
			m_queTakeItemNoticeData.pop();
		}
	}
}
void CX2NoticeManager::CloseItemNotice(int iNum)
{

	//각 알림 별 메세지의 컨트롤 얻기
	CKTDGUIStatic *pStatic[MAX_NOTICE_NUM];
	for( UINT i = 0; i < MAX_NOTICE_NUM; ++i)
	{
		if( NULL != m_pDlgTakeItemBackBg[i])
		{
			pStatic[i] = (CKTDGUIStatic*)m_pDlgTakeItemBackBg[i]->GetControl(L"Notice_Message");
		}
	}

	//공지 제거되면 하나씩 아래로 밀도록 설정
	for( UINT i = iNum; i< MAX_NOTICE_NUM-1; i++)
	{
		if( NULL != pStatic[i] && NULL != pStatic[i+1])
		{
			pStatic[i]->GetString(0)->msg = pStatic[i+1]->GetString(0)->msg;
			m_fTakeItemNoticeTime[i] = m_fTakeItemNoticeTime[i+1];
		}
	}

	//몇번째 것을 끄던 하나는 무조건 없어져야 하기 때문에 젤 위에건 삭제
	if( NULL != pStatic[MAX_NOTICE_NUM-1] )
	{
		pStatic[MAX_NOTICE_NUM-1]->GetString(0)->msg = L"";
		m_fTakeItemNoticeTime[MAX_NOTICE_NUM-1] = 0.f;
	}
}