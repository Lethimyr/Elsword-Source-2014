CREATE PROCEDURE [dbo].[up_query_user_nickname]
	@strLogin_ [nvarchar](20)
AS
set nocount on
set transaction isolation level read uncommitted

select
    Nick
from dbo.NickNames
where
    Login = @strLogin_


