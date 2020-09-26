CREATE PROCEDURE [dbo].[GIA_game_item_list_select_part]
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081226. microcat.

L"{ call dbo.GIA_game_item_list_select_part }"

{ call dbo.GIA_game_item_list_select_part }
인자 없음

n return ( @1, @2, @3 )
@1 ; SetItemID int
@2 ; ItemType int
    0 ; 영구/기간
    1 ; 수량
@3 ; PartItemID int
*/

select
    a.GoodsID
,   a.ItemType
,   b.GoodsID
from
(
    select
        GoodsID
    ,   SetGoodsNum
    ,   ItemType
    from dbo.GoodsInfoList
    where
        IsSubSetGoods = 1
)
as a
join
(
    select
        GoodsID
    ,   SetGoodsNum
    from dbo.GoodsInfoList
    where
        IsSubSetGoods = 0
    and GoodsID % 10 = 0
    and Kind in (0, 1, 2, 8, 9, 13)
)
as b
on
    a.SetGoodsNum = b.SetGoodsNum


