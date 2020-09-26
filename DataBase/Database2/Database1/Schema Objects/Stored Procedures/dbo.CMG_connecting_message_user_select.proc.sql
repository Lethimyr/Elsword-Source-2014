CREATE PROCEDURE [dbo].[CMG_connecting_message_user_select]
	@iLoginUID_ [int]
AS
/*microcat
작성일 : 2008년 3월 25일
사용 : 유저에게 디스플레이될 메세지 ID 리턴
*/
set nocount on

declare
    @sdtDate smalldatetime
select
    @sdtDate = convert(nvarchar(10), getdate(), 120)

select
    MessageID
from dbo.CMGConnectingMessageUser
with (nolock)
where
    LoginUID = @iLoginUID_
and @sdtDate <= LimitDate
and Readed = 0
option (maxdop 1)

begin tran
    update a
    with (updlock)
    set
        Readed = 1
    from dbo.CMGConnectingMessageUser as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and @sdtDate <= LimitDate
    and Readed = 0
    option (maxdop 1)
    if @@error <> 0
        begin    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:


