CREATE PROCEDURE [dbo].[up_admin_select_del_normal_item]
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
    a.GoodsName
,   @strLogin_
,   b.BuyerLogin
,   b.ItemID
,   b.ItemUID
,   b.RegDate
,   b.StartDate
,   b.EndDate
,   b.Period
,   b.DelDate
from dbo.GoodsInfolist
as a
join [Log].dbo.IPLOItemPeriodItem
as b
on
    a.GoodsID = b.ItemID
WHERE
    b.LoginUID = @iLoginUID
order by
    b.DelDate desc


