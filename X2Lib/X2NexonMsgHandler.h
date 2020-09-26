#pragma once
#ifdef _NEXON_KR_

class CX2NexonMsgHandler : public CNMEventHandler
{
	public:
		CX2NexonMsgHandler();
		virtual ~CX2NexonMsgHandler();

		//	인증서버와의 접속이 끊어졌을 경우에 발생
		virtual void OnAuthConnectionClosedEvent( CNMAuthConnectionClosedEvent * pEvent );


#ifndef NEW_MESSENGER
		//	메신저서버와의 접속이 연결되었을 경우에 발생
		virtual void OnMessengerReplyEvent( CNMMessengerReplyEvent * pEvent );

		//	메신저서버와의 접속이 끊어졌을 경우에 발생
		virtual void OnMsgConnectionClosedEvent( CNMMsgConnectionClosedEvent * pEvent );

		// 새로운 친구 등록 요청이 왔을 때 발생?
		virtual void OnRequestNewFriendEvent( CNMRequestNewFriendEvent* pEvent );

		// 각종 정보의 변경(내 정보, 친구 리스트의 변경, 내 길드 리스트의 변경, 쪽지함의 변경)이 있을 때 
		// 발생하는 이벤트이다. 
		// 내가 내 정보를 변경하거나, 친구 또는 그룹을 추가/삭제/변경할 경우에도 발생한다.
		virtual void OnRefreshEvent( CNMRefreshEvent* pEvent );

		// 친구 추가 성공, 실패, 쪽지 관련 성공 여부 이벤트 처리
		virtual void OnCustomMessageEvent( CNMCustomMessageEvent* pEvent );

		// 친구의 정보가 변경되었을 경우 발생하는 이벤트
		virtual void OnFriendInfoChangedEvent( CNMFriendInfoChangedEvent* pEvent );

#endif // #ifndef NEW_MESSENGER

		// 세션이 생성된 경우 발생하는 이벤트
		virtual void OnGSSessionCreatedEvent( CNMGSSessionCreatedEvent* pEvent );

		// 세션에 대한 각종 정보가 변경되었을 경우 발생.
		virtual void OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent );

		// 시스템 메시지나 알림 메시지는 메시지 전송 결과나 대화 참가자의 상태 변화 등을 알려주는 메시지를 담고 있다.
		virtual void OnCSChatMessageReceivedEvent( CNMCSChatMessageReceivedEvent* pEvent );

		// 세션이 최종적으로 세팅 되었을 경우(초대한 친구까지 Join하여 Operation이 가능한 경우) 발생
		virtual void OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent );

		// 세션이 종료되었을 경우 발생. 해당 세션의 Serial Key는 더 이상 유효하지 않게 된다.
		// 메신저 로그아웃 등 세션이 완전히 끊긴 경우 발생하므로, 이 이벤트가 온 경우에는 채팅창을 닫도록 한다.
		virtual void OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent );

		// 여러 가지 이유로 세션이 정상적으로 세팅되지 않았을 경우 발생.
		virtual void OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent );

	
		

	private:
};
#endif _NEXON_KR_
