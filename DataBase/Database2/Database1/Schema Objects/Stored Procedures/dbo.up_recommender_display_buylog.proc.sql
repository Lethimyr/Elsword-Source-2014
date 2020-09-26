CREATE PROCEDURE [dbo].[up_recommender_display_buylog]
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
    b.GoodsName
,   a.Price
,   a.RegDate
from
(
    select
        ItemID
    ,   Price
    ,   RegDate
    from dbo.RecommenderLog
    where
        LoginUID = @iLoginUID
)
as a
join dbo.GoodsInfoList
as b
on
    a.ItemID = b.GoodsID


