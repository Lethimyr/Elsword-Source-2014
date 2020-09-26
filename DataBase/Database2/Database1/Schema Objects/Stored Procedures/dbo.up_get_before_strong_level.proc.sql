CREATE PROCEDURE [dbo].[up_get_before_strong_level]
	@iGoodsID_ [int]
AS
exec dbo.up_log_set_proc_count 'up_get_before_strong_level'

SET NOCOUNT ON

DECLARE @strGoodsName   nvarchar(50), 
        @iPresentLevel  int, 
        @iPresentKind   int,
        @iBeforeLevel   int,
        @iBeforeGoodsID int, 
        @iCharType      int


IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_)
BEGIN
    SELECT -1
    RETURN
END
--현재 GoodsID의 이름을 구한다. 
SELECT @strGoodsName = GoodsName,
       @iPresentLevel = StrongLevel,
       @iPresentKind = StrongItemKind, 
       @iCharType = CharType
FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_

SET @iBeforeLevel = @iPresentLevel - 1

IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsName = @strGoodsName AND StrongLevel = @iBeforeLevel AND StrongItemKind = @iPresentKind  AND CharType = @iCharType )
BEGIN
    SELECT -1
    RETURN
END
ELSE
BEGIN
   --그 이름으로 다음 단계의 GoodsID를 구한다.
    SELECT @iBeforeGoodsID = GoodsID FROM dbo.GoodsInfoList(NOLOCK)  WHERE GoodsName = @strGoodsName AND StrongLevel = @iBeforeLevel AND StrongItemKind = @iPresentKind AND CharType = @iCharType

   SELECT  @iBeforeGoodsID
   RETURN
END


