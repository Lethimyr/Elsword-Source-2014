CREATE PROCEDURE [dbo].[up_enchant_acc_get_nextitem]
	@iGoodsID_ [int]
AS
SET NOCOUNT ON

DECLARE 
@iNextGoodsID int,
@iStrongLevel int,
@iStrongItemKind int,
@iCharType int

SELECT @iNextGoodsID = -1, @iStrongItemKind = -1, @iStrongLevel = -1000, @iCharType = -1

-- 이 아이템이 강화 가능한지 확인
SELECT @iStrongLevel = StrongLevel, @iStrongItemKind = StrongItemKind, @iCharType = CharType
	FROM dbo.GoodsInfoList(NOLOCK)
		WHERE GoodsID = @iGoodsID_ AND unableStrong = 0

IF @iStrongLevel = -1000 --아이템이 존재하지 않음
	BEGIN
		GOTO END_PROC
	END

-- 다음 단계가 있는지 확인
SELECT @iNextGoodsID = GoodsID, @iStrongLevel = StrongLevel, @iStrongItemKind = StrongItemKind
	FROM dbo.GoodsInfoList(NOLOCK)
		WHERE CharType = @iCharType AND StrongLevel = @iStrongLevel + 1 AND StrongItemKind = @iStrongItemKind
            AND (UnableStrong = 0 OR @iStrongLevel = 8)

END_PROC:
SELECT @iNextGoodsID, @iStrongItemKind, @iStrongLevel, @iCharType


