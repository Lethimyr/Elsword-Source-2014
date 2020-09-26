CREATE FUNCTION [dbo].[fnGetItemType](@iItemID_ [int])
RETURNS [real] WITH SCHEMABINDING
AS 
BEGIN
    DECLARE @iItemType int
    SET @iItemType = 0  -- default로 기간제를 리턴한다.
    select @iItemType = ItemType from dbo.GoodsInfoList(NOLOCK) where GoodsID = @iItemID_
    RETURN @iITemType
END


