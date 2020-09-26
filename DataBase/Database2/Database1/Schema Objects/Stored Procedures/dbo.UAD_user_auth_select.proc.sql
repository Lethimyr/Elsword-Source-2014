CREATE PROCEDURE [dbo].[UAD_user_auth_select]
	@strLogin_ [nvarchar](20)
AS
/*microcat
작성일 : 2008년 4월 7일
사용 : dbot에서 유저 인증을 확인합니다.
*/
set nocount on

select
    a.Login
,   isnull(b.AuthLevel, 0) as AuthLevel
,   isnull(convert(nvarchar(16), c.LimitDate, 120), N'None') as LimitDate
,   Status
from dbo.Users as a
with (nolock)
left outer join dbo.UAGUserAuth as b
with (nolock)
on a.LoginUID = b.LoginUID
left outer join dbo.UAGUserAuthDate as c
with (nolock)
on b.LoginUID = c.LoginUID
left outer join dbo.UAGUserAuthStatus as d
with (nolock)
on c.LoginUID = d.LoginUID
where a.Login = @strLogin_


