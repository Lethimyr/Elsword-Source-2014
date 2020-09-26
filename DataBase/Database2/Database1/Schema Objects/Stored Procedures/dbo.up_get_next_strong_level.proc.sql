CREATE PROCEDURE [dbo].[up_get_next_strong_level]
	@iGoodsID_ [int]
AS
exec dbo.up_log_set_proc_count 'up_get_next_strong_level'

SET NOCOUNT ON

DECLARE @strGoodsName       nvarchar(50), 
        @iPresentLevel      int, 
        @iPresentKind       int,
        @iNextLevel         int, 
        @iNextGoodsID       int, 
        @iNextItemKind      int,
        @iCharType          int,
        @iMagicPaperGoodsID int,
        @iUnableStrong      bit


IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_)
BEGIN
    SELECT -1,  0, -1, -1, -1
    RETURN
END
--현재 GoodsID의 이름을 구한다. 
SELECT @strGoodsName = GoodsName,
       @iPresentLevel = StrongLevel,
       @iPresentKind = StrongItemKind, 
       @iCharType = CharType, 
       @iUnableStrong = UnableStrong
FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_

--강화 안되는 아이템이면 리턴 false
IF @iUnableStrong = 1
BEGIN
    SELECT -1, 0,-1, -1, -1
    RETURN
END

SET @iNextLevel = @iPresentLevel + 1

IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE Gem > 0 AND StrongLevel = @iNextLevel )
BEGIN
    SELECT -1, 0,-1, -1, -1
    RETURN
END
--마법서의 GoodsID를 구한다.
SELECT  @iMagicPaperGoodsID = GoodsID 
FROM dbo.GoodsInfoList(NOLOCK) WHERE Gem > 0 AND StrongLevel = @iNextLevel

IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsName = @strGoodsName AND StrongLevel = @iNextLevel AND StrongItemKind = @iPresentKind  AND CharType = @iCharType )
BEGIN
    SELECT -1, 0, -1, -1, -1
    RETURN
END
ELSE
BEGIN
   --그 이름으로 다음 단계의 GoodsID를 구한다.
    SELECT @iNextGoodsID = GoodsID FROM dbo.GoodsInfoList(NOLOCK)  WHERE GoodsName = @strGoodsName AND StrongLevel = @iNextLevel AND StrongItemKind = @iPresentKind AND CharType = @iCharType

   SELECT @iNextLevel, @iNextGoodsID, @iPresentKind, @iMagicPaperGoodsID, @iCharType
   RETURN
END


