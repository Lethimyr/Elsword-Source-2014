CREATE PROCEDURE [dbo].[AAA_item_exist]
	@iItemID_ [int],
	@bSetItem_ [bit],
	@bItemType_ [bit],
	@iOK [int] = null OUTPUT
AS
set nocount on
set transaction isolation level read uncommitted

if @@nestlevel < 2
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.GoodsInfoList
    where
        GoodsID = @iItemID_
    and IsSubSetGoods = @bSetItem_
    and ItemType = @bItemType_
)
begin    select @iOK = -2    goto end_proc    end

end_proc:


