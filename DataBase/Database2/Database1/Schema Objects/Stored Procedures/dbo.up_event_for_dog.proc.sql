CREATE PROCEDURE [dbo].[up_event_for_dog]
	@strLogin_ [nvarchar](20),
	@iCommandType_ [int] = 0,
	@iLuckyBag [int] = 0,
	@iOK [int] = 0
AS
/*
프로시저 설명 : 다음의 프로시저 용법 외에는 모두 에러처리가 됩니다.
exec dbo.up_event_for_dog 'microcat'        --복주머니 개수 조회
exec.dbo.up_event_for_dog 'microcat', 1     --강아지 머리 지급
exec.dbo.up_event_for_dog 'microcat', 2     --강아지 꼬리 지급
exec.dbo.up_event_for_dog 'microcat', 3     --강아지 펫 지급
exec.dbo.up_event_for_dog 'microcat', 4, 1  --복주머니 수량 1개 차감
exec.dbo.up_event_for_dog 'microcat', 4, 5  --복주머니 수량 5개 차감

리턴값 설명 : 아래 참고
@iOK, @iLuckyBag
@iOK
 ( 0) : 프로시저 성공
 (-1) : 해당 유저가 존재하지 않음
 (-2) : 차감할 복주머니가 충분하지 않음
 (-3) : 아이템 지급 범위를 벗어나거나 복주머니가 충분하지 않음
 (-4이하) : 트랜잭션 에러
@iLuckyBag
 @iOK값이 0일 경우 정상적인 값이 출력되며
 그 이외의 값일 경우에는 0이 출력됩니다.
*/

SET NOCOUNT ON;

DECLARE @strSQL nvarchar(4000), @iLuckyBagGoodsID int, @iLuckyBagFromDB int, @iNeedLuckyBag int, @iPeriod int
SELECT @strSQL = N'SELECT @iOK, @iLuckyBag', @iLuckyBagGoodsID = 13780, @iLuckyBagFromDB = 0, @iNeedLuckyBag = 30, @iPeriod = 7

IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_)
    BEGIN   SET @iOK = -1   GOTO END_PROC   END

SELECT @iLuckyBagFromDB = Duration FROM dbo.DurationItemObjectList(NOLOCK)
    WHERE OwnerLogin = @strLogin_ AND GoodsID = @iLuckyBagGoodsID

IF @iCommandType_ = 0
    BEGIN
        SET @iLuckyBag = @iLuckyBagFromDB
        GOTO END_PROC
    END
IF @iCommandType_ = 4
    BEGIN
        SET @iLuckyBag = @iLuckyBagFromDB - @iLuckyBag
        IF @iLuckyBag < 0
            BEGIN   SELECT @iOK = -2, @iLuckyBag = 0    GOTO END_PROC   END
        BEGIN TRAN
            UPDATE dbo.DurationItemObjectList WITH (ROWLOCK)
                SET Duration = @iLuckyBag
                    WHERE OwnerLogin = @strLogin_ AND GoodsID = @iLuckyBagGoodsID
            IF @@ERROR <> 0 BEGIN   SET @iOK = -10 GOTO FAIL_TRAN  END
        COMMIT TRAN
        GOTO END_PROC
    END
ELSE IF @iCommandType_ NOT BETWEEN 1 AND 3 OR @iLuckyBagFromDB < @iNeedLuckyBag
    BEGIN   SELECT @iOK = -3, @iLuckyBag = 0   GOTO END_PROC   END

DECLARE @ItemList TABLE
(
    No      int identity(1, 1) not null primary key,
    ItemID  int not null
)

IF @iCommandType_ = 3
    BEGIN
        INSERT INTO @ItemList(ItemID) VALUES(22030)
    END
ELSE
    BEGIN
        INSERT INTO @ItemList(ItemID) VALUES(CASE @iCommandType_ WHEN 1 THEN 21930 WHEN 2 THEN 21940 END)
        INSERT INTO @ItemList(ItemID) VALUES(CASE @iCommandType_ WHEN 1 THEN 21950 WHEN 2 THEN 21960 END)
        INSERT INTO @ItemList(ItemID) VALUES(CASE @iCommandType_ WHEN 1 THEN 21970 WHEN 2 THEN 21980 END)

        IF EXISTS(SELECT * FROM dbo.Characters(NOLOCK) WHERE Login = @strLogin_ AND CharType = 3)
            BEGIN
                INSERT INTO @ItemList(ItemID) VALUES(CASE @iCommandType_ WHEN 1 THEN 21990 WHEN 2 THEN 22000 END)
            END
    END

DECLARE @iLoop int, @iLoopMax int, @sdtNow smalldatetime
SELECT @iLoop = 1, @iLoopMax = MAX(No), @sdtNow = GetDate() FROM @ItemList

BEGIN TRAN
    WHILE @iLoop <= @iLoopMAX
        BEGIN
            DECLARE @iItemID int
            SELECT @iItemID = ItemID FROM @ItemList WHERE No = @iLoop
            IF NOT EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND ItemID = @iItemID AND Period > 0)
                BEGIN
                    INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                        VALUES(@strLogin_, '_EventDog_', @iItemID, @sdtNow, @sdtNow - 1, @sdtNow - 1, @iPeriod)
                    IF @@ERROR <> 0 BEGIN   SET @iOK = -11 GOTO FAIL_TRAN  END
                END
            ELSE
                BEGIN
                    UPDATE dbo.GoodsObjectList WITH (UPDLOCK)
                        SET EndDate = EndDate + @iPeriod, Period = Period + @iPeriod
                            WHERE OwnerLogin = @strLogin_ AND ItemID = @iItemID
                    IF @@ERROR <> 0 BEGIN   SET @iOK = -12 GOTO FAIL_TRAN  END
                END
            SET @iLoop = @iLoop + 1
        END
    SET @iLuckyBag = @iLuckyBagFromDB - @iNeedLuckyBag
    UPDATE dbo.DurationItemObjectList WITH (ROWLOCK)
        SET Duration = @iLuckyBag
            WHERE OwnerLogin = @strLogin_ AND GoodsID = @iLuckyBagGoodsID
    IF @@ERROR <> 0 BEGIN   SET @iOK = -13 GOTO FAIL_TRAN  END
COMMIT TRAN
GOTO END_PROC

FAIL_TRAN:
SET @iLuckyBag = 0
ROLLBACK TRAN

END_PROC:
exec dbo.sp_executesql @strSQL, N'@iOK int, @iLuckyBag int', @iOK, @iLuckyBag


