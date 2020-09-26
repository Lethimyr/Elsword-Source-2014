CREATE PROCEDURE [dbo].[up_recommender_display_point]
	@strLogin_ [nvarchar](20)
AS
set nocount on
set transaction isolation level read uncommitted

declare
    @iLoginUID int
select
    @iLoginUID = LoginUID
from dbo.Users
where
    Login = @strLogin_

select
    c.Nick
,   a.Point
from
(
    select
        LoginUID
    ,   Point
    from dbo.RecommenderPointList
    where
        LoginUID = @iLoginUID
)
as a
JOIN dbo.Users
as b
on
    a.LoginUID = b.LoginUID
join dbo.NickNames
as c
on
    b.Login = c.Login


