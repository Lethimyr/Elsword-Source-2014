CREATE PROCEDURE [dbo].[CMD_connecting_message_delete]
	@iMessageID_ [int],
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2008년 3월 27일
사용 : dbot에서 메세지 삭제합니다.
*/
set nocount on

if not exists
(
    select *
    from dbo.CMGConnectingMessage
    with (nolock)
    where
        MessageID = @iMessageID_
)
begin    select @iOK = -1    goto end_proc    end

begin tran
    delete a
    with (updlock)
    from dbo.CMGConnectingMessage as a
    with (updlock)
    where
        MessageID = @iMessageID_
    if @@error <> 0
    begin    select @iOK = -101, @iError = @@error    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK as OK


