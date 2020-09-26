CREATE PROCEDURE [dbo].[CMG_connecting_message_select]
	@iMessageID_ [int]
AS
/*microcat
작성일 : 2008년 3월 25일
사용 : 유저에게 디스플레이될 메세지 리턴
*/
set nocount on

select
    Message
from dbo.CMGConnectingMessage
with (nolock)
where
    MessageID = @iMessageID_
option (maxdop 1)


