CREATE PROCEDURE [dbo].[up_event_insert_item_for_mobileshop]
	@strLogin_ [nvarchar](20),
	@strMobileCode_ [nvarchar](16),
	@iItemID_ [int],
	@iDuration_ [int] = 0
AS
/*
프로시저 설명
입력 : @strLogin_ nvarchar(20) --넷마블 ID를 입력하면 됩니다.
       @strMobileCode_ nvarchar(16) --모바일 코드를 입력하면 됩니다.
출력 : SELECT @iOK, RETURN @iOK --@iOK값으로 프로시저의 성공/실패 여부를 확인할 수 있습니다.
@iOK ( 0) : 성공/아이템이 정상적으로 지급되었음
     (-1) : 해당 유저가 존재하지 않음
     (-2) : 아이템이 존재하지 않음
     (-3이하) : 트랜잭션 작업 중 문제 발생

예제 : exec dbo.up_event_insert_item_for_mobileshop 'microcat', '1234567890123456'
*/

SET NOCOUNT ON;

DECLARE @iOK int
SELECT @iOK = 0

IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_)
    BEGIN   SET @iOK = -1   GOTO END_PROC   END

IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iItemID_)
    BEGIN   SET @iOK = -2   GOTO END_PROC   END

DECLARE @iITemType int,
        @iFactor int,
        @bIsSubSetGoods bit,
        @iNumSetGoods int,
        @strMark nvarchar(20)

SELECT @iItemType = ItemType,
       @iFactor = CASE ItemType WHEN 0 THEN @iDuration_ WHEN 1 THEN (CASE @iDuration_ WHEN 0 THEN Duration ELSE @iDuration_ END) END,
       @bIsSubSetGoods = IsSubSetGoods,
       @iNumSetGoods = SetGoodsNum,
       @strMark = '_EventMobileShop_'
    FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iItemID_

IF @iItemType = 0
    BEGIN
        IF NOT EXISTS(SELECT * FROM dbo.ItemPriceByPeriod(NOLOCK) WHERE ItemID = @iItemID_ AND Period = @iFactor)
            BEGIN   SET @iOK = -2   GOTO END_PROC   END
    END

DECLARE @ItemList TABLE
(
    No int Identity(1, 1) not null primary key,
    ItemID int not null
)

IF @bIsSubSetGoods = 1
    BEGIN
        INSERT INTO @ItemList(ItemID)
            SELECT GoodsID
                FROM dbo.GoodsInfoList(NOLOCK)
                    WHERE SetGoodsNum = @iNumSetGoods AND
                            GoodsID <> @iItemID_ AND
                            GoodsID = GoodsID / 10 * 10 AND
                            (Kind < 10 OR Kind = 13) AND
                            Kind <> 3 AND Kind <> 4
    END
ELSE
    BEGIN
        INSERT INTO @ItemList(ItemID) VALUES(@iItemID_)
    END

DECLARE @iLoop int, @iLoopMax int
SELECT @iLoop = 1, @iLoopMax = MAX(No) FROM @ItemList

BEGIN TRAN
    WHILE @iLoop <= @iLoopMax
        BEGIN
            DECLARE @iItemID int
            SELECT @iItemID = ItemID FROM @ItemList WHERE No = @iLoop

            IF @iItemType = 0
                BEGIN
                    DECLARE @sdtNow smalldatetime
                    SET @sdtNow = GetDate()
                    IF NOT EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND ItemID = @iItemID AND Period > 0)
                        BEGIN
                            INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                                VALUES(@strLogin_, @strMark, @iItemID, @sdtNow, @sdtNow - 1, @sdtNow - 1, @iFactor)
                            IF @@ERROR <> 0 BEGIN   SET @iOK = -3   GOTO FAIL_TRAN  END
                        END
                    ELSE
                        BEGIN
                            UPDATE dbo.GoodsObjectList WITH (UPDLOCK)
                                SET EndDate = CASE WHEN RegDate <= StartDate THEN DateAdd(dd, @iFactor, EndDate) ELSE EndDate END,
                                    Period = Period + @iFactor
                                    WHERE OwnerLogin = @strLogin_ AND ItemID = @iItemID AND Period > 0
                            IF @@ERROR <> 0 BEGIN   SET @iOK = -4   GOTO FAIL_TRAN  END
                        END
                END
            ELSE
                BEGIN
                    IF NOT EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND GoodsID = @iItemID)
                        BEGIN
                            INSERT INTO dbo.DurationItemObjectList(OwnerLogin, BuyerLogin, GoodsID, Duration)
                                VALUES(@strLogin_, @strMark, @iItemID, @iFactor)
                            IF @@ERROR <> 0 BEGIN   SET @iOK = -5   GOTO FAIL_TRAN  END
                        END
                    ELSE
                        BEGIN
                            UPDATE dbo.DurationItemObjectList WITH (UPDLOCK)
                                SET Duration = Duration + @iFactor
                                    WHERE OwnerLogin = @strLogin_ AND GoodsID = @iItemID
                            IF @@ERROR <> 0 BEGIN   SET @iOK = -6   GOTO FAIL_TRAN  END
                        END
                END
            SET @iLoop = @iLoop + 1
        END

    INSERT INTO dbo.EventMobileShop(Login, MobileCode, ItemCode, Factor) VALUES(@strLogin_, @strMobileCode_, @iItemID_, @iFactor)
    IF @@ERROR <> 0 BEGIN   SET @iOK = -5   GOTO FAIL_TRAN  END

COMMIT TRAN
GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK
RETURN @iOK


