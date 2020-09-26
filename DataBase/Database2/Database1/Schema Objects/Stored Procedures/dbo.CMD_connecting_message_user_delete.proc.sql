CREATE PROCEDURE [dbo].[CMD_connecting_message_user_delete]
	@iLoginUID_ [int],
	@iMessageID_ [int],
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2008년 3월 28일
사용 : dbot에서 유저 삭제합니다.
*/
set nocount on

if not exists
(
    select *
    from dbo.CMGConnectingMessageUser
    with (nolock)
    where
        LoginUID = @iLoginUID_
    and MessageID = @iMessageID_
)
begin    select @iOK = -1    goto end_proc    end

begin tran
    delete a
    with (updlock)
    from dbo.CMGConnectingMessageUser as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and MessageID = @iMessageID_
    if @@error <> 0
    begin    select @iOK = -101, @iError = @@error    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK as OK


