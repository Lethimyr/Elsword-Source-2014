CREATE PROCEDURE [dbo].[up_get_revival_break_item]
	@iGoodsID_ [int]
AS
exec dbo.up_log_set_proc_count 'up_get_revival_break_item'

SET NOCOUNT ON

DECLARE @iPresentLevel      int, 
        @iPresentKind       int,
        @iRevivalLevel      int, 
        @iRevivalGoodsID    int, 
        @iCharType          int,
        @iCategory          int,
        @iUpLevel           int


IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_)
BEGIN
    SELECT -1
    RETURN
END
--현재 GoodsID의 레벨과 종류를 구한다.
SELECT @iPresentLevel = StrongLevel,
       @iPresentKind = StrongItemKind, 
       @iCharType = CharType,
       @iCategory = Category
FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_

SET @iUpLevel = 3
SET @iRevivalLevel = @iPresentLevel + @iUpLevel

IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE Category = @iCategory AND StrongLevel = @iRevivalLevel AND StrongItemKind = @iPresentKind  AND CharType = @iCharType AND UnableStrong = 0 AND Cash = 0)
BEGIN
    SELECT -1
    RETURN
END
ELSE
BEGIN
   --그 이름으로 소생 단계의 GoodsID를 구한다.
    SELECT @iRevivalGoodsID = GoodsID FROM dbo.GoodsInfoList(NOLOCK)  WHERE Category = @iCategory AND StrongLevel = @iRevivalLevel AND StrongItemKind = @iPresentKind AND CharType = @iCharType AND UnableStrong = 0 AND Cash = 0

    SELECT  @iRevivalGoodsID
   RETURN
END


