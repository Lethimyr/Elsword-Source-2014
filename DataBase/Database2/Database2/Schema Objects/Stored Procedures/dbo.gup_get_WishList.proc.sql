CREATE PROCEDURE [dbo].[gup_get_WishList]
	@iUserUID	bigint 

AS
SET NOCOUNT ON;

SELECT SlotNo, ItemID
	FROM dbo.GItemWishList WITH (NOLOCK)
		WHERE UserUID = @iUserUID
			AND ItemID > 0


