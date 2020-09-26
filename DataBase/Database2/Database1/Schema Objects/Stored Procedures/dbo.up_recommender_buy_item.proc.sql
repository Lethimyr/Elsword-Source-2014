CREATE PROCEDURE [dbo].[up_recommender_buy_item]
	@strLogin_ [nvarchar](20),
	@iItemID_ [int],
	@iFactor_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

DECLARE @iLoginUID int SELECT @iLoginUID = -1
SELECT @iLoginUID = LoginUID
    FROM dbo.Users(NOLOCK)
        WHERE Login = @strLogin_
IF @iLoginUID = -1
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

DECLARE @iPoint int SELECT @iPoint = -1
SELECT @iPoint = Point
    FROM dbo.RecommenderPointList(NOLOCK)
        WHERE LoginUID = @iLoginUID
IF @iPoint = -1
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

DECLARE @iPrice int, @iItemType int, @bUpdated int, @iSellCount int
SELECT @iPrice = -1, @iItemType = -1, @bUpdated = 0
SELECT @iItemType = ItemType, @iPrice = Price, @iSellCount = SellCount
    FROM dbo.RecommenderItemList(NOLOCK)
        WHERE ItemID = @iItemID_ AND Factor = @iFactor_
IF @iSellCount is not NULL AND @iSellCount = 0
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

IF @iPrice = -1
    BEGIN    SELECT @iOK = -4    GOTO END_PROC    END

IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iItemID_ AND ItemType = @iITemType)
    BEGIN    SELECT @iOK = -5    GOTO END_PROC    END

IF @iPoint < @iPrice
    BEGIN    SELECT @iOK = -6    GOTO END_PROC    END

IF @iItemType = 0
BEGIN
    IF @iFactor_ <> -1 AND EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND ItemID = @iItemID_ AND Period > 0)
        BEGIN    SELECT @bUpdated = 1    END
END
ELSE IF @iItemType = 1
BEGIN
    IF EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND GoodsID = @iItemID_)
        BEGIN    SELECT @bUpdated = 1    END
END

DECLARE @sdtNow smalldatetime, @iItemUID int
SELECT @sdtNow = GetDate(), @iItemUID = -1
BEGIN TRAN
    IF @iItemType = 0
        BEGIN
            IF @bUpdated = 0
                BEGIN
                    INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                        SELECT @strLogin_, '__Recommender__', @iItemID_, @sdtNow, @sdtNow -1, @sdtNow - 1, @iFactor_
                    IF @@ERROR <> 0
                        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END
                    SELECT @iItemUID = @@Identity
                END
            ELSE IF @bUpdated = 1
                BEGIN
                    UPDATE dbo.GoodsObjectList WITH (UPDLOCK)
                        SET EndDate = EndDate + @iFactor_, Period = Period + @iFactor_, @iItemUID = ItemUID
                            WHERE Ownerlogin = @strLogin_ AND ItemID = @iItemID_ AND Period > 0
                    IF @@ERROR <> 0
                        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
                END
        END
    ELSE IF @iItemType = 1
        BEGIN
            IF @bUpdated = 0
                BEGIN
                    INSERT INTO dbo.DurationItemObjectList(OwnerLogin, BuyerLogin, GoodsID, Duration, RegDate)
                        SELECT @strLogin_, '__Recommender__', @iItemID_, @iFactor_, @sdtNow
                    IF @@ERROR <> 0
                        BEGIN    SELECT @iOK = -13    GOTO FAIL_TRAN    END
                    SELECT @iItemUID = @@Identity
                END
            ELSE IF @bUpdated = 1
                BEGIN
                    UPDATE dbo.DurationItemObjectList WITH (UPDLOCK)
                        SET Duration = Duration + @iFactor_, @iItemUID = DurationItemUID
                            WHERE Ownerlogin = @strLogin_ AND GoodsID = @iItemID_
                    IF @@ERROR <> 0
                        BEGIN    SELECT @iOK = -14    GOTO FAIL_TRAN    END
                END
        END

    IF @iItemUID = -1
        BEGIN    SELECT @iOK = -15    GOTO FAIL_TRAN    END

    UPDATE dbo.RecommenderPointList WITH(ROWLOCK)
        SET Point = @iPoint - @iPrice
            WHERE LoginUID = @iLoginUID
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -16    GOTO FAIL_TRAN    END

    IF @iSellCount is not NULL AND @iSellCount > 0
        BEGIN
            UPDATE dbo.RecommenderItemList WITH (ROWLOCK)
                SET SellCount = @iSellCount - 1
                    WHERE ItemID = @iItemID_ AND Factor = @iFactor_
            IF @@ERROR <> 0
                BEGIN    SELECT @iOK = -17    GOTO FAIL_TRAN    END
        END

    INSERT INTO dbo.RecommenderLog(LoginUID, ItemUID, ItemID, ItemType, Factor, Price)
        SELECT @iLoginUID, @iItemUID, @iItemID_, @iItemType, @iFactor_, @iPrice
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -18    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


