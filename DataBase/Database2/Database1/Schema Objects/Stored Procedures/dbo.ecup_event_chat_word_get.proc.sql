CREATE PROCEDURE [dbo].[ecup_event_chat_word_get]
	@iUID_ [int]
AS
/*microcat
작성일 : 2007년 9월 11일
사용 : 이벤트 발동 단어 리턴
*/
set nocount on;

--시간대의 유일성은 보장된다.
select  Word
    from dbo.ECEventChatWord with (nolock)
        where UID = @iUID_


