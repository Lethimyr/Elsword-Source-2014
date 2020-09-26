CREATE PROCEDURE [dbo].[LSG_login_server_connect_status_update]
	@strLogin_ [nvarchar](20),
	@bConnecting_ [bit],
	@iOK [int] = 0
AS
set nocount on

declare
    @iLoginUID int
select
    @iLoginUID = LoginUID
from dbo.Users
with (nolock)
where
    Login = @strLogin_

if @iLoginUID is null
begin    select @iOK = -1    goto end_proc    end

declare
    @bNew bit
select
    @bNew = 1
if exists
(
    select *
    from dbo.LSGLoginServerConnectStatus
    with (nolock)
    where
        LoginUID = @iLoginUID
)
begin
    select @bNew = 0
end

begin tran
    if @bNew = 0
    begin
        update a
        with (updlock)
        set
            a.Connecting = @bConnecting_
        from dbo.LSGLoginServerConnectStatus
        as a
        with (updlock)
        where
            LoginUID = @iLoginUID
        if @@error <> 0
        begin    select @iOK = -101    goto fail_tran    end
    end
    else if @bNew = 1
    begin
        insert into dbo.LSGLoginServerConnectStatus
        with (updlock)
        (
            LoginUID
        ,   Connecting
        )
        select
            @iLoginUID
        ,   @bConnecting_
        if @@error <> 0
        begin    select @iOK = -102    goto fail_tran    end
    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


