CREATE PROCEDURE [dbo].[up_admin_select_normal_item]
	@strLogin_ [nvarchar](20)
AS
exec dbo.up_log_set_proc_count 'up_admin_select_normal_item'

SET NOCOUNT ON

SELECT a.GoodsName, b.Ownerlogin, b.BuyerLogin, b.ItemID, b.ItemUID, b.RegDate, b.StartDate, b.EndDate, b.Period
    FROM dbo.GoodsInfolist as a(nolock) JOIN dbo.GoodsObjectlist as b(NOLOCK) ON a.GoodsID = b.ItemID
        WHERE b.Ownerlogin = @strLogin_ ORDER BY b.RegDate DESC


