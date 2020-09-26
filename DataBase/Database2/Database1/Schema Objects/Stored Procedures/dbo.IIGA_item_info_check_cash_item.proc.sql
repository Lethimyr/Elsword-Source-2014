CREATE PROCEDURE [dbo].[IIGA_item_info_check_cash_item]
	@iItemID_ [int],
	@iFactor_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090113. microcat.

L"{ call dbo.IIGA_item_info_check_cash_item ( %d, %d ) }"

{ call dbo.IIGA_item_info_check_cash_item ( @1, @2 ) }
@1 ; @iItemID_ int
@2 ; @iFactor_ int

1 return ( @1, @2 )
@1 ; ItemType int
@2 ; Price int
*/

declare
    @iItemType int
,   @iPrice int

select
    @iItemType = ItemType
,   @iPrice = Price
from dbo.GoodsInfoList
where
    GoodsID = @iItemID_
and GoodsID % 10 = 0
and ShowWeb = 1
and Cash = 1

if @iItemType = 0
begin
    select
        @iPrice = Price
    from dbo.ItemPriceByPeriod
    where
        ItemID = @iItemID_
    and Period = @iFactor_
end

select
    @iItemType
,   @iPrice
where
    @iItemType is not null


