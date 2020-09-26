CREATE PROCEDURE [dbo].[up_recommender_display_userlist]
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
        RecommenderUID
    ,   Point
    from dbo.RecommenderList
    where
        RecommendeeUID = @iLoginUID
)
as a
join dbo.Users
as b
on
    a.RecommenderUID = b.LoginUID
join dbo.NickNames
as c
on
    b.Login = c.Login


