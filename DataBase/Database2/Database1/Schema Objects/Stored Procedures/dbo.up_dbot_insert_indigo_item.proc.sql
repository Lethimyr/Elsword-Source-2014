CREATE PROCEDURE [dbo].[up_dbot_insert_indigo_item]
	@iRank_ [nvarchar](20),
	@iItemID_ [int],
	@iDuration_ [int]
AS
SET NOCOUNT ON

-- 입력받은 @iDuration 값의 유효성 여부 검사
IF @iDuration_ = 0
BEGIN 
    SELECT -2 AS OK, '입력받은 Duration 값이 유효하지 않음 : ' + @iDuration_ as Msg
END

-- 아이템 아이디가 유효한지 확인
IF NOT EXISTS( SELECT * FROM dbo.GoodsInfoList( NOLOCK ) WHERE GoodsID = @iItemID_ )
BEGIN 
    SELECT -1 as OK, '존재하지 않는 아이템 아이디 입니다.' as Msg
    RETURN
END

-- 아이템의 추가정보를 얻음
DECLARE @iItemType int,
        @iCharType int,
        @strItemName nvarchar(100)

SELECT @iItemType = ItemType, @iChartype = CharType, @strItemName = GoodsName FROM GoodsInfoList(NOLOCK) WHERE GoodsID = @iItemID_

-- 입력받은 @iDuration 값의 유효성 여부 검사
IF @iItemType = 1 AND @iDuration_ < 0   -- 수량 아이템인데 duration이 음수로 입력됨
BEGIN
    SELECT -2 AS OK, '입력받은 아이템 수량이 유효하지 않음 : ' + CAST(@iDuration_ AS NVARCHAR(5) ) as Msg
    RETURN
END

BEGIN TRAN

    INSERT INTO IndigoReward( Rank, ItemID, CharType, ItemName, ItemType, Duration )
        VALUES( @iRank_, @iItemID_, @iCharType, @strItemName, @iItemType, @iDuration_ )

    IF @@ERROR <> 0
        GOTO FAIL_PROC

COMMIT TRAN

SELECT 0 as OK, '성공적으로 추가되었습니다.' as Msg
RETURN

FAIL_PROC:
ROLLBACK TRAN
SELECT -99 AS OK, '알 수 없는 오류' as Msg


