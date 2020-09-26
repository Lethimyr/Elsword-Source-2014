CREATE PROCEDURE [dbo].[UADB_user_auth_select_log]
	@strLogin_ [nvarchar](20),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090107. microcat.
*/

declare
    @iLoginUID int
select
    @iLoginUID = LoginUID
from dbo.Users
where
    Login = @strLogin_

select
    dbo.IPs_IPn(ipn) as IP
,   convert(nvarchar(23), RegDate, 121) as RegDate
,   AuthLevel2 as AuthLevel
,   isnull(convert(nvarchar(16), LimitDate2, 120), N'None') as LimitDate
,   Status2 as Status
from dbo.UALUserAuth
where
    LoginUID = @iLoginUID
order by
    RegDate


