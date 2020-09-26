CREATE PROCEDURE [dbo].[DSG_divide_sell_item_list_select]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20080508. microcat.
*/

declare
    @iDivideFactor int
,   @iCashPerGem int
select
    @iDivideFactor = Variable1
,   @iCashPerGem = Variable2
from dbo.GlobalVariables
where
    No = 4

select
    GoodsID
,   case
        when Cash = 1 or Gem > 0 then 2
        when Crystal > 0 then 3
        else 1
    end
,   case
        when Gem > 0 then Gem
        when Crystal > 0 then Crystal
        else Price
    end
,   Duration
,   case Cash
        when 1 then isnull(@iCashPerGem, 200.0)
        else @iDivideFactor
    end
from dbo.GoodsInfoList
where
    ItemType = 1


