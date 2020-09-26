CREATE PROCEDURE [dbo].[up_get_unable_strong_item]
	@iGoodsID_ [int],
	@iDownLevel_ [int]
AS
exec dbo.up_log_set_proc_count 'up_get_unable_strong_item'

SET NOCOUNT ON

DECLARE @iUnableStrongGoodsID   int, 
        @iStrongItemKind        int,
        @iCharType              int,
        @iStrongLevel           int, 
        @iUnableStrongLevel     int

IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_)
BEGIN
    SELECT -1
    RETURN
END
--현재 GoodsID의 이름을 구한다. 
SELECT   @iStrongLevel = StrongLevel,
         @iStrongItemKind = StrongItemKind, 
         @iCharType = CharType
FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_

SET @iUnableStrongLevel = @iStrongLevel - @iDownLevel_

IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE UnableStrong = 1 AND StrongLevel = @iUnableStrongLevel AND StrongItemKind = @iStrongItemKind  AND CharType = @iCharType )
BEGIN
    SELECT -1
    RETURN
END
ELSE
BEGIN
   --그 이름으로 다음 단계의 GoodsID를 구한다.
    SELECT @iUnableStrongGoodsID = GoodsID FROM dbo.GoodsInfoList(NOLOCK)  WHERE UnableStrong = 1 AND StrongLevel = @iUnableStrongLevel AND StrongItemKind = @iStrongItemKind  AND CharType = @iCharType

   SELECT  @iUnableStrongGoodsID
   RETURN
END


