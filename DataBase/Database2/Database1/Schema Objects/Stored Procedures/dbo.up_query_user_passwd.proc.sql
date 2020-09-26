CREATE PROCEDURE [dbo].[up_query_user_passwd]
	@strLogin_ [nvarchar](20)
AS
set nocount on

select
    Passwd
,   Login
from dbo.Users
with (nolock)
where
    Login = @strLogin_


