#include "stdafx.h"
#include ".\x2nexonmsghandler.h"

#ifdef _NEXON_KR_

CX2NexonMsgHandler::CX2NexonMsgHandler()
{
}

CX2NexonMsgHandler::~CX2NexonMsgHandler()
{
}



//
//	인증서버와의 접속이 끊어졌을 경우에 발생
//
void CX2NexonMsgHandler::OnAuthConnectionClosedEvent( CNMAuthConnectionClosedEvent * pEvent )
{
	//ClientUtil::AddLog( g_log, _T("%s, %d, %s"), _T(__FUNCTION__), pEvent->uType, ClientUtil::AuthResultToString( static_cast<NMLoginAuthReplyCode>( pEvent->uType ) ) );
}

#ifndef NEW_MESSENGER
//{{AFX
//
//	메신저서버와의 접속이 연결되었을 경우에 발생
//
void CX2NexonMsgHandler::OnMessengerReplyEvent( CNMMessengerReplyEvent * pEvent )
{
	//ClientUtil::AddLog( g_log, _T("%s, %d, %s"), _T(__FUNCTION__), pEvent->nResultCode, ClientUtil::MessengerResultToString( static_cast<NMMessengerReplyCode>( pEvent->nResultCode ) ) );

	if ( pEvent->nResultCode == kMessengerReplyOK || pEvent->nResultCode == kMessengerReplyNewUser )
	{

		//if ( pEvent->nResultCode == kMessengerReplyOK )
		//{
		//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"메신저 접속 성공", g_pMain->GetNowState() );
		//}

		if ( g_pData != NULL && g_pData->GetMessenger() != NULL && 
			g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL)
		{
			g_pData->GetMessenger()->ResetUserList();
		}
	}
}
//
//	메신저서버와의 접속이 끊어졌을 경우에 발생
//
void CX2NexonMsgHandler::OnMsgConnectionClosedEvent( CNMMsgConnectionClosedEvent * pEvent )
{
	//ClientUtil::AddLog( g_log, _T("%s, %d, %s"), _T(__FUNCTION__), pEvent->uType, ClientUtil::MessengerConnectionClosedEventToString( pEvent->uType ) );
}


void CX2NexonMsgHandler::OnRequestNewFriendEvent( CNMRequestNewFriendEvent* pEvent )
{
	if ( g_pData != NULL && g_pData->GetMessenger() != NULL )
	{
		g_pData->GetMessenger()->OnRequestNewFriendEvent( pEvent  );
		//g_pData->GetMessenger()->OnRequestNewFriendEvent( pEvent->rnfData.keyToVirtual.uVirtualIDCode, pEvent->rnfData.uSerialNo, 
		//	pEvent->rnfData.szFromNickName, pEvent->rnfData.szMessage  );
	}
}

void CX2NexonMsgHandler::OnRefreshEvent( CNMRefreshEvent* pEvent )
{
	//친구리스트, 쪽지 차단 리스트, 나의 버추얼 유저 리스트 등이 변경되었으므로 
	//새로 읽어와서 반영해 주어야 한다.
	if ( pEvent->uType == CNMRefreshEvent::kType_UserDataList )	
	{
		if ( g_pData != NULL && g_pData->GetMessenger() != NULL && 
			g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
		{
			g_pData->GetMessenger()->SetRefresh();
			g_pData->GetMessenger()->ResetUserList();
		}
	}
}

void CX2NexonMsgHandler::OnCustomMessageEvent( CNMCustomMessageEvent* pEvent )
{
	switch( pEvent->msgData.nCode )
	{
		case kCustomMsgCode_RequestFriendOK:		// 친구 등록 요청 성공
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_368 ), g_pMain->GetNowState() );
			}
			break;
		case kCustomMsgCode_RequestFriendFailed:	// 친구 등록 요청 실패
			{										// TODO : 친구 등록 요청 실패에 대한 메시지(pEvent->msgData.szMessage) 보여줌.
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300),pEvent->msgData.szMessage, g_pMain->GetNowState() );
			}	
			break;
		case kCustomMsgCode_NoteReply: 				// 쪽지 보내기 결과
			{
				
			}
			break;
	}

}


void CX2NexonMsgHandler::OnFriendInfoChangedEvent( CNMFriendInfoChangedEvent* pEvent )
{
	g_pData->GetMessenger()->OnFriendInfoChangedEvent( pEvent );
}

//}}AFX
#endif NEW_MESSENGER

// 09.07.02 태완 : 이 아래의 함수들은 그냥 제거한 경우 Link Error가 나기 때문에 이를 피하기 위해서 NEW_MESSENGER일 때는 Dummy화.
// 넥슨 모듈 완전히 떼면 제거해도 된다.
void CX2NexonMsgHandler::OnGSSessionCreatedEvent( CNMGSSessionCreatedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnGSSessionCreatedEvent( pEvent );
#endif	
}

void CX2NexonMsgHandler::OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnGSSessionInfoChangedEvent( pEvent );
#endif
}

void CX2NexonMsgHandler::OnCSChatMessageReceivedEvent( CNMCSChatMessageReceivedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnCSChatMessageReceivedEvent( pEvent );
#endif
}

void CX2NexonMsgHandler::OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnGSSessionEstablishedEvent( pEvent );
#endif
}

void CX2NexonMsgHandler::OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnGSSessionClosedEvent( pEvent );
#endif
}

void CX2NexonMsgHandler::OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnGSSessionFailedEvent( pEvent );
#endif
}




#endif _NEXON_KR_
