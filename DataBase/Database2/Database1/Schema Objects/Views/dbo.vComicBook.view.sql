CREATE VIEW [dbo].[vComicBook]
AS
SELECT  b.SerialNumber
,       a.CouponName
,       b.CouponType
,       c.ItemName
,       c.bUse
    FROM dbo.ComicBookCouponType as a(NOLOCK)
        JOIN dbo.ComicBookCouponList as b(NOLOCK) on a.CouponType = b.CouponType
        JOIN dbo.ComicBookItemInfo as c(NOLOCK) on b.SerialNumber = c.SerialNumber


