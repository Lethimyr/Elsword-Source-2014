CREATE PROCEDURE [dbo].[up_web_event_coupon_item]
	@strLogin_ [nvarchar](20),
	@strSN_ [nvarchar](20),
	@iOK [int] = 0
AS
SET NOCOUNT ON;

IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

DECLARE @iPatternType int, @bUsed bit
SELECT @iPatternType = 0x80000000, @bUsed = 0
SELECT @iPatternType = PatternType, @bUsed = Used FROM dbo.WebEventCoupon(NOLOCK) WHERE SerialNumber = @strSN_
IF @iPatternType = 0x80000000
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

IF @bUsed = 1
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

DECLARE @ItemList TABLE
(
    No int not null identity(1, 1) primary key,
    ItemType tinyint not null,
    ItemID int not null,
    Factor int not null
)

INSERT INTO @ItemList(ItemType, ItemID, Factor)
    SELECT ItemType, ItemID, Factor FROM dbo.WebEventCouponPatterns(NOLOCK) WHERE PatternType = @iPatternType
IF NOT EXISTS(SELECT * FROM @ItemList)
    BEGIN    SELECT @iOK = -4    GOTO END_PROC    END

--지정된 ItemType
--0(GP)
--1(무제한 일반 아이템)
--2(기간 일반 아이템)
--3(수량 아이템)

DECLARE @iLoop int, @iLoopMax int, @sdtNow smalldatetime, @strBuyer nvarchar(20)
SELECT @iLoop = 1, @iLoopMax = MAX(No), @sdtNow = GetDate(), @strBuyer = '___WebEventCoupon___' FROM @ItemList

BEGIN TRAN
WHILE @iLoop <= @iLoopMax
    BEGIN
        DECLARE @iITemType tinyint, @iItemID int, @iFactor int
        SELECT @iItemType = ItemType, @iItemID = ItemID, @iFactor = Factor FROM @ItemList WHERE No = @iLoop

        IF @iItemType = 0
            BEGIN
                UPDATE dbo.Users WITH (UPDLOCK)
                    SET GamePoint = GamePoint + @iFactor
                        WHERE Login = @strLogin_
                IF @@ERROR <> 0
                    BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END
            END
        ELSE IF @iItemType = 1
            BEGIN
                INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                    SELECT @strLogin_, @strBuyer, @iItemID, @sdtNow, @sdtNow, @sdtNow - 1, -1
                IF @@ERROR <> 0
                    BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
            END
        ELSE IF @iItemType = 2
            BEGIN
                IF EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND ItemID = @iItemID AND Period > 0)
                    BEGIN
                        UPDATE dbo.GoodsObjectList WITH (UPDLOCK)
                            SET EndDate = DateAdd(dd, @iFactor, EndDate), Period = Period + @iFactor
                                WHERE OwnerLogin = @strLogin_ AND ItemID = @iItemID AND Period > 0
                        IF @@ERROR <> 0
                            BEGIN    SELECT @iOK = -13    GOTO FAIL_TRAN    END
                    END
                ELSE
                    BEGIN
                        INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                            SELECT @strLogin_, @strBuyer, @iItemID, @sdtNow, @sdtNow - 1, @sdtNow - 1, @iFactor
                        IF @@ERROR <> 0
                            BEGIN    SELECT @iOK = -14    GOTO FAIL_TRAN    END
                    END
            END
        ELSE IF @iItemType = 3
            BEGIN
                IF EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND GoodsID = @iItemID)
                    BEGIN
                        UPDATE dbo.DurationItemObjectList WITH (UPDLOCK)
                            SET Duration = Duration + @iFactor
                                WHERE OwnerLogin = @strLogin_ AND GoodsID = @iItemID
                        IF @@ERROR <> 0
                            BEGIN    SELECT @iOK = -15    GOTO FAIL_TRAN    END
                    END
                ELSE
                    BEGIN
                        INSERT INTO dbo.DurationItemObjectList(OwnerLogin, BuyerLogin, GoodsID, Duration)
                            SELECT @strLogin_, @strBuyer, @iItemID, @iFactor
                        IF @@ERROR <> 0
                            BEGIN    SELECT @iOK = -16    GOTO FAIL_TRAN    END
                    END
            END
        ELSE
            BEGIN    SELECT @iOK = -17    GOTO FAIL_TRAN    END

        SELECT @iLoop = @iLoop + 1
    END

IF LEFT(@strSN_, 1) <> '!'
    BEGIN
        UPDATE dbo.WebEventCoupon WITH (ROWLOCK)
            SET Used = 1
                WHERE SerialNumber = @strSN_
        IF @@ERROR <> 0
            BEGIN    SELECT @iOK = -18    GOTO FAIL_TRAN    END
    END

INSERT INTO dbo.LogWebEventCoupon(Login, SerialNumber, PatternType)
    SELECT @strLogin_, @strSN_, @iPatternType
IF @@ERROR <> 0
    BEGIN    SELECT @iOK = -19    GOTO FAIL_TRAN    END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


