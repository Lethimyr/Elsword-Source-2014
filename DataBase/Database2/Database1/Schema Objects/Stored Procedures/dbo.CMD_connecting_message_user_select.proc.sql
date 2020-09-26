CREATE PROCEDURE [dbo].[CMD_connecting_message_user_select]
AS
/*microcat
작성일 : 2008년 3월 27일
사용 : dbot에서 유저 퍼갑니다
*/
set nocount on

select
    b.Login
,   a.LoginUID
,   a.MessageID
,   convert(nvarchar(10), a.LimitDate, 120) as LimitDate
,   a.Readed
from dbo.CMGConnectingMessageUser as a
with (nolock)
join dbo.Users as b
with (nolock)
on a.LoginUID = b.LoginUID
option (maxdop 1)


