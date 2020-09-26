CREATE PROCEDURE [dbo].[up_update_disconnect_info]
	@strLogin_ [nvarchar](20),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

if not exists
(
    select *
    from dbo.Users
    where
        Login = @strLogin_
)
begin    select @iOK = -1    goto end_proc    end

begin transaction

update a
with (updlock)
set
    Connecting = 0
from dbo.Users
as a
with (updlock)
where
    Login = @strLogin_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
--select
--    @iOK


