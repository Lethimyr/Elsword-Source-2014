CREATE PROCEDURE [dbo].[CMG_connecting_message_schedule]
AS
/*microcat
작성일 : 2008년 3월 25일
사용 : 유저에게 디스플레이된 메세지 정리
*/
set nocount on

declare
    @sdtDate smalldatetime
select
    @sdtDate = convert(nvarchar(10), getdate(), 120)

begin tran
    delete a
    with (updlock)
    from dbo.CMGConnectingMessageUser as a
    with (updlock)
    where
        LimitDate < @sdtDate
    or  Readed = 1
    option (maxdop 1)
    if @@error <> 0
        begin    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:


