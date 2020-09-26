CREATE PROCEDURE [dbo].[up_admin_select_duration_item]
	@strLogin_ [nvarchar](20)
AS
exec dbo.up_log_set_proc_count 'up_admin_select_duration_item'

SEt NOCOUNT ON

SELECT a.GoodsName, b.Ownerlogin, b.BuyerLogin, b.GoodsID, b.DurationItemUID, b.Duration, b.RegDate
FROM dbo.GoodsInfolist as a(nolock) JOIN dbo.DurationItemObjectlist as b(NOLOCK) ON 
a.GoodsID = b.GoodsID WHERE b.Ownerlogin = @strLogin_ Order by b.RegDate desc


