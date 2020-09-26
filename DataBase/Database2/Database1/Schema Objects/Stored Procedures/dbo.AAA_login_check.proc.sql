CREATE PROCEDURE [dbo].[AAA_login_check]
	@iLoginUID_ [int],
	@iOK [int] = null OUTPUT,
	@strLogin [nvarchar](20) = null OUTPUT
AS
set nocount on
set transaction isolation level read uncommitted

if @@nestlevel < 2
begin    select @iOK = -1    goto end_proc    end

select
    @strLogin = Login
from dbo.Users
where
    LoginUID = @iLoginUID_
option (maxdop 1)

if @strLogin is null
begin    select @iOK = -2    goto end_proc    end

end_proc:


