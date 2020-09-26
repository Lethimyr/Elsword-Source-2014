CREATE PROCEDURE [dbo].[CMD_connecting_message_select]
AS
/*microcat
작성일 : 2008년 3월 27일
사용 : dbot에서 메세지 퍼갑니다
*/
set nocount on

select
    MessageID
,   Message
from dbo.CMGConnectingMessage
with (nolock)
option (maxdop 1)


