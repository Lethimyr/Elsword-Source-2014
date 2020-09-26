CREATE PROCEDURE [dbo].[NMW_net_marble_goodsinfolist_insert]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081010. microcat.
*/

if not exists
(
    select *
    from dbo.GlobalVariables
    where
        No = 7
    and Variable1 in ( 1 )
)
begin    goto end_proc    end

begin transaction

truncate table dbo.NMWNetMarbleGoodsInfoList

if @@error <> 0
begin    goto fail_tran    end

insert into dbo.NMWNetMarbleGoodsInfoList
with (updlock)
(
    GoodsID
,   GoodsName
,   Description
,   IsLimited
,   Duration
,   CharType
,   Kind
,   ItemType
,   ShowWeb
,   HotCount
,   Price
,   IsNewGoods
,   Cash
,   GoodsClass
,   Img
,   RegDate
,   IsSubSetGoods
,   SetGoodsNum
,   Recom
,   Category
,   Attack
,   Defence
,   HPInc
,   LastingPrice
)
Select
    AA.GoodsID
,   AA.GoodsName
,   AA.Description
,   AA.IsLimited
,   AA.Duration
,   AA.CharType
,   AA.Kind
,   AA.ItemType
,   AA.ShowWeb
,   AA.HotCount
,   AA.Price
,   AA.IsNewGoods
,   AA.Cash
,   AA.GoodsClass
,   AA.Img
,   AA.RegDate
,   AA.IsSubSetGoods
,   AA.SetGoodsNum
,   AA.Recom
,   AA.Category
,   AA.attack
,   AA.Defence
,   AA.HPInc 
,   lastingPrice
From
(
    SELECT
        A.GoodsID
    ,   A.GoodsName
    ,   A.Description
    ,   A.IsLimited
    ,   A.Duration
    ,   A.CharType
    ,   A.Kind
    ,   A.ItemType
    ,   A.ShowWeb
    ,   A.HotCount
    ,   A.Price
    ,   A.IsNewGoods
    ,   A.Cash
    ,   A.GoodsClass
    ,   A.Img
    ,   A.RegDate
    ,   A.IsSubSetGoods
    ,   A.SetGoodsNum
    ,   A.Recom
    ,   A.Category,attack
    ,   A.Defence
    ,   A.HPInc   
    ,   isNull(B.price,-1)  As lastingPrice
    FROM dbo.GoodsInfoList
    as A
    with (nolock)
    Left outer Join dbo.ItemPriceByPeriod
    as B
    with (nolock)
    On
        A.GoodsID = B.itemid
    And B.Period = -1
    WHERE
        A.ShowWeb = 1
    AND A.cash = 1
    AND A.Kind in (50,51,52,53)
    AND A.Category not in (19,-1,-2)
    Union
    SELECT
        A.GoodsID
    ,   A.GoodsName
    ,   A.Description
    ,   A.IsLimited
    ,   A.Duration
    ,   A.CharType
    ,   A.Kind
    ,   A.ItemType
    ,   A.ShowWeb
    ,   A.HotCount
    ,   A.Price
    ,   A.IsNewGoods
    ,   A.Cash
    ,   A.GoodsClass
    ,   A.Img
    ,   A.RegDate
    ,   A.IsSubSetGoods
    ,   A.SetGoodsNum
    ,   A.Recom
    ,   A.Category,attack
    ,   A.Defence
    ,   A.HPInc   
    ,   isNull(B.price,-1)  As lastingPrice
    FROM dbo.GoodsInfoList
    as A
    with (nolock)
    Left outer Join dbo.ItemPriceByPeriod
    as B
    with (nolock)
    On
        A.GoodsID = B.itemid
    And B.Period = -1
    WHERE
        A.ShowWeb = 1
    AND A.cash = 1
    AND A.Category = 387
)
as AA
ORDER BY
    AA.RegDate asc
option (maxdop 1)

if @@error <> 0
begin    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:


