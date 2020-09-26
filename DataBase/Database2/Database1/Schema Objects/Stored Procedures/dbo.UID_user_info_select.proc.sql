CREATE PROCEDURE [dbo].[UID_user_info_select]
	@strLogin_ [nvarchar](20)
AS
set nocount on

/*
20080826. microcat.
*/

select
    a.Login
,   a.LoginUID
,   isnull(b.Nick, N'______Unregistered______') as Nick
,   Sex
,   convert(nvarchar(16), FirstLogin, 120) as FirstLogin
,   convert(nvarchar(16), LastLogin, 120) as LastLogin
,   PlayTime
,   GamePoint
,   IPAddress
,   Connecting
from dbo.Users
as a
with (nolock)
left outer join dbo.Nicknames
as b
with (nolock)
on
    a.Login = b.Login
where
    a.Login = @strLogin_


