CREATE PROCEDURE [dbo].[up_admin_select_del_duration_item]
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
,   b.DelDate
FROM dbo.GoodsInfolist
as a
join [Log].dbo.ICLOItemCountItem
as b
on
    a.GoodsID = b.ItemID
where
    b.LoginUID = @iLoginUID
order by
    b.DelDate desc


