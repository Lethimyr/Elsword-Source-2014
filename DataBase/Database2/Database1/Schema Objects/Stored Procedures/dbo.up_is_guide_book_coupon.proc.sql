CREATE PROCEDURE [dbo].[up_is_guide_book_coupon]
	@strSN_ [nvarchar](50),
	@iOK [int] = 0
AS
SET NOCOUNT ON;

DECLARE @strItemName nvarchar(50)
SELECT  @strItemName = ItemName
    FROM dbo.ComicBookItemInfo(NOLOCK)
        WHERE SerialNumber = @strSN_

IF @strItemName is NULL
   BEGIN    SELECT @iOk = -1    END

IF @strItemName = N'GC공략집'
   BEGIN    SELECT @iOk = 1    END

SELECT @iOK


