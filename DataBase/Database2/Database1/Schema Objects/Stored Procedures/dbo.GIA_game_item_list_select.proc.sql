CREATE PROCEDURE [dbo].[GIA_game_item_list_select]
AS
set nocount on
set transaction isolation level read uncommitted

select
    GoodsID
,   ItemType
from dbo.GoodsInfoList
where
    IsSubSetGoods = 0
option (maxdop 1)


