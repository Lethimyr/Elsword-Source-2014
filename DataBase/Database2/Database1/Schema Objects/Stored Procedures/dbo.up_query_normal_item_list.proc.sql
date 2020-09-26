CREATE PROCEDURE [dbo].[up_query_normal_item_list]
	@strLogin_ [nvarchar](20),
	@bShowBuyer [bit] = 1
AS
SET NOCOUNT ON

IF @bShowBuyer = 1
    BEGIN
        select  a.BuyerLogin
,               a.ItemUID
,               a.ItemID
,               a.RegDate
,               a.StartDate
,               a.EndDate
,               a.Period
,               isnull(b.StrongLevel, 0)
            from dbo.GoodsObjectList as a with (nolock)
            left outer join dbo.GStrongItem as b with (nolock)
                on a.ItemUID = b.ItemUID
                    where a.OwnerLogin = @strLogin_
    END
ELSE
    BEGIN
        select  a.ItemUID
,               a.ItemID
,               a.RegDate
,               a.StartDate
,               a.EndDate
,               a.Period
,               isnull(b.StrongLevel, 0)
            from dbo.GoodsObjectList as a with (nolock)
            left outer join dbo.GStrongItem as b with (nolock)
                on a.ItemUID = b.ItemUID
                    where a.OwnerLogin = @strLogin_
    END


