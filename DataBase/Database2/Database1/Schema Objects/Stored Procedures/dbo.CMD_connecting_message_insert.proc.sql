CREATE PROCEDURE [dbo].[CMD_connecting_message_insert]
	@iMessageID_ [int],
	@strMessage_ [nvarchar](4000),
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2008년 3월 27일
사용 : dbot에서 메세지 입력합니다
*/
set nocount on

if exists
(
    select *
    from dbo.CMGConnectingMessage
    with (nolock)
    where
        MessageID = @iMessageID_
)
begin    select @iOK = -1    goto end_proc    end

begin tran
    insert into dbo.CMGConnectingMessage
    with (updlock)
    (
        MessageID
    ,   Message
    )
    select
        @iMessageID_
    ,   @strMessage_
    if @@error <> 0
    begin    select @iOK = -101, @iError = @@error    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK as OK


