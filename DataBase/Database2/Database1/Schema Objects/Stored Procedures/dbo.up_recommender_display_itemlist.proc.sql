CREATE PROCEDURE [dbo].[up_recommender_display_itemlist]
AS
SET NOCOUNT ON;

SELECT a.ItemID, b.GoodsName, b.Description, a.Factor, a.Price
    FROM dbo.RecommenderItemList as a(NOLOCK)
        JOIN dbo.GoodsInfoList as b(NOLOCK) on a.ItemID = b.GoodsID


