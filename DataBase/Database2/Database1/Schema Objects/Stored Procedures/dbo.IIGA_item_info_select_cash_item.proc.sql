CREATE PROCEDURE [dbo].[IIGA_item_info_select_cash_item]
	@iLoginUID_ [int],
	@iItemType_ [int],
	@iItemID_ [int],
	@iFactor_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090113. microcat.

L"{ call dbo.IIGA_item_info_select_cash_item ( %d, %d, %d, %d ) }"

{ call dbo.IIGA_item_info_select_cash_item ( @1, @2, @3, @4 ) }
@1 ; @iLoginUID_ int
@2 ; @iItemType_ int
@3 ; @iItemID_ int
@4 ; @iFactor_ int

n return ( @1, @2, @3 )
@1 ; ItemType int
@2 ; ItemID int
@2 ; Factor int
*/

declare
    @dtDateA datetime
select
    @dtDateA = current_timestamp

if exists
(
select *
from dbo.PackageInfoList
where
    ItemID = @iItemID_
)
begin
    select
        ItemType
    ,   PackageItemID
    ,   Factor
    from dbo.PackageInfoList
    where
        ItemID = @iItemID_

    insert into [Log].dbo.IILOItemInfoCashItem
    with (updlock)
    (
	    LoginUID
    ,	RegDateA
    ,	MainItemType
    ,	MainItemID
    ,	MainFactor
    ,	SubItemType
    ,	SubItemID
    ,	SubFactor
    )
    select
        @iLoginUID_
    ,   @dtDateA
    ,   @iItemType_
    ,   @iItemID_
    ,   @iFactor_
    ,   ItemType
    ,   PackageItemID
    ,   Factor
    from dbo.PackageInfoList
    where
        ItemID = @iItemID_
end
else if exists
(
    select *
    from dbo.CashPackageInfoDetailList
    where
        ItemID = @iItemID_
)
begin
    select
        ItemType
    ,   SubItemID
    ,   Factor
    from dbo.CashPackageInfoDetailList
    where
        ItemID = @iItemID_

    insert into [Log].dbo.IILOItemInfoCashItem
    with (updlock)
    (
	    LoginUID
    ,	RegDateA
    ,	MainItemType
    ,	MainItemID
    ,	MainFactor
    ,	SubItemType
    ,	SubItemID
    ,	SubFactor
    )
    select
        @iLoginUID_
    ,   @dtDateA
    ,   @iItemType_
    ,   @iItemID_
    ,   @iFactor_
    ,   ItemType
    ,   SubItemID
    ,   Factor
    from dbo.CashPackageInfoDetailList
    where
        ItemID = @iItemID_
end
else if exists
(
    select *
    from dbo.GoodsInfoList
    where
        GoodsID = @iItemID_
    and IsSubSetGoods = 1
    and SetGoodsNum > -1
)
begin
    declare
        @iSetID int
    select
        @iSetID = SetGoodsNum
    from dbo.GoodsInfoList
    where
        GoodsID = @iItemID_

    select
        @iItemType_
    ,   GoodsID
    ,   @iFactor_
    from dbo.GoodsInfoList
    where
        SetGoodsNum = @iSetID
    and GoodsID % 10 = 0
    and IsSubSetGoods = 0
    and Kind in ( 0, 1, 2, 8, 9, 13 )

    insert into [Log].dbo.IILOItemInfoCashItem
    with (updlock)
    (
	    LoginUID
    ,	RegDateA
    ,	MainItemType
    ,	MainItemID
    ,	MainFactor
    ,	SubItemType
    ,	SubItemID
    ,	SubFactor
    )
    select
        @iLoginUID_
    ,   @dtDateA
    ,   @iItemType_
    ,   @iItemID_
    ,   @iFactor_
    ,   @iItemType_
    ,   GoodsID
    ,   @iFactor_
    from dbo.GoodsInfoList
    where
        SetGoodsNum = @iSetID
    and GoodsID % 10 = 0
    and IsSubSetGoods = 0
    and Kind in ( 0, 1, 2, 8, 9, 13 )
end
else
begin
    select
        @iItemType_
    ,   GoodsID
    ,   @iFactor_
    from dbo.GoodsInfoList
    where
        GoodsID = @iItemID_

    insert into [Log].dbo.IILOItemInfoCashItem
    with (updlock)
    (
	    LoginUID
    ,	RegDateA
    ,	MainItemType
    ,	MainItemID
    ,	MainFactor
    ,	SubItemType
    ,	SubItemID
    ,	SubFactor
    )
    select
        @iLoginUID_
    ,   @dtDateA
    ,   @iItemType_
    ,   @iItemID_
    ,   @iFactor_
    ,   @iItemType_
    ,   GoodsID
    ,   @iFactor_
    from dbo.GoodsInfoList
    where
        GoodsID = @iItemID_
end


