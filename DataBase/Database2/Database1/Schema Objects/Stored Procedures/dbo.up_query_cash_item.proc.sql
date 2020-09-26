CREATE PROCEDURE [dbo].[up_query_cash_item]
	@iLoginUID_ [int],
	@bGift_ [bit] = 0
AS
SET NOCOUNT ON;

IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE LoginUID = @iLoginUID_)
    BEGIN    GOTO END_PROC    END

DECLARE @ItemList1 TABLE
(
    No int not null identity(1, 1) primary key,
    OwnerLogin nvarchar(20) not null,
    BuyerLogin nvarchar(20) not null,
    ItemType int not null,
    ItemID int not null,
    Factor int not null,
    RegDate smalldatetime not null,
    PackageItem bit not null,
    SetItem bit not null,
    GiftItem bit not null
)

INSERT INTO @ItemList1(OwnerLogin, BuyerLogin, ItemType, ItemID, Factor, RegDate, PackageItem, SetItem, GiftItem)
    SELECT c.Login, d.Login, a.ItemType, a.ItemID, a.Factor, b.RegDate, a.PackageItem, a.SetItem, CASE WHEN b.BuyerLoginUID = b.OwnerLoginUID THEN 0 ELSE 1 END
        FROM dbo.CashItemDetailList as a(NOLOCK)
            JOIN dbo.CashItemList as b(NOLOCK) ON a.ItemUID = b.ItemUID AND b.OwnerLoginUID = CASE @bGift_ WHEN 0 THEN b.BuyerLoginUID WHEN 1 THEN b.OwnerLoginUID END AND b.OwnerLoginUID = @iLoginUID_
            JOIN dbo.Users as c(NOLOCK) ON b.OwnerLoginUID = c.LoginUID
            JOIN dbo.Users as d(NOLOCK) ON b.BuyerLoginUID = d.LoginUID

IF NOT EXISTS(SELECT * FROM @ItemList1)
    BEGIN    GOTO END_PROC    END

DECLARE @ItemList2 TABLE
(
    No int not null identity(1, 1) primary key,
    OwnerLogin nvarchar(20) not null,
    BuyerLogin nvarchar(20) not null,
    ItemType int not null,
    OriginalItemID int not null,
    ItemUID int null,
    ItemID int not null,
    Factor int not null,
    RegDate smalldatetime not null,
    StartDate smalldatetime null,
    EndDate smalldatetime null,
    GiftItem bit not null
)

DECLARE @iLoop int, @iLoopMax int, @sdtNow smalldatetime
SELECT @iLoop = 1, @iLoopMax = MAX(No), @sdtNow = GetDate() FROM @ItemList1
WHILE @iLoop <= @iLoopMax
BEGIN
    DECLARE @strOwnerLogin nvarchar(20), @strBuyerLogin nvarchar(20), @iItemType int, @iItemID int, @iFactor int, @sdtRegDate smalldatetime, @bPackageItem bit, @bSetItem bit, @bGiftItem bit
    SELECT @strOwnerLogin = OwnerLogin, @strBuyerLogin = BuyerLogin, @iItemType = ItemType, @iItemID = ItemID, @iFactor = Factor, @sdtRegDate = RegDate, @bPackageItem = PackageItem, @bSetItem = SetItem, @bGiftItem = GiftItem
        FROM @ItemList1 WHERE No = @iLoop

    IF @bPackageItem = 1
        BEGIN
            INSERT INTO @ItemList2(OwnerLogin, BuyerLogin, ItemType, OriginalItemID, ItemID, Factor, RegDate, GiftItem)
                SELECT @strOwnerLogin, @strBuyerLogin, ItemType, @iItemID, SubItemID, Factor, @sdtRegDate, @bGiftItem
                    FROM dbo.CashPackageInfoDetailList(NOLOCK)
                        WHERE ItemID = @iItemID AND (SupplyCount > 0 OR SupplyCount = -1) AND (SupplyDate >= @sdtNow OR SupplyDate = 0)
        END
    ELSE IF @bSetItem = 1
        BEGIN
            INSERT INTO @ItemList2(OwnerLogin, BuyerLogin, ItemType, OriginalItemID, ItemID, Factor, RegDate, GiftItem)
                SELECT @strOwnerLogin, @strBuyerLogin, ItemType, 0, GoodsID, @iFactor, @sdtRegDate, @bGiftItem
                    FROM dbo.GoodsInfoList(NOLOCK)
                    WHERE SetGoodsNum = (SELECT SetGoodsNum FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iItemID)
                            AND GoodsID <> @iItemID AND (GoodsID % 10) = 0 AND Kind <> 3 AND Kind <> 4 AND (Kind < 10 OR Kind = 13)
        END
    ELSE
        BEGIN
            INSERT INTO @ItemList2(OwnerLogin, BuyerLogin, ItemType, OriginalItemID, ItemID, Factor, RegDate, GiftItem)
                SELECT @strOwnerLogin, @strBuyerLogin, @iItemType, 0, @iItemID, @iFactor, @sdtRegDate, @bGiftItem
        END
    SELECT @iLoop = @iLoop + 1
END

SELECT @iLoop = 1, @iLoopMax = MAX(No) FROM @ItemList2
BEGIN TRAN
    WHILE @iLoop <= @iLoopMax
        BEGIN
            DECLARE @iOriginalItemID int, @iItemUID int, @sdtStartDate smalldatetime, @sdtEndDate smalldatetime
            SELECT @strOwnerLogin = OwnerLogin, @strBuyerLogin = BuyerLogin, @iItemType = ItemType, @iOriginalItemID = OriginalItemID, @iItemID = ItemID, @iFactor = Factor, @sdtRegDate = RegDate,
                   @sdtStartDate = RegDate, @sdtEndDate = RegDate
                FROM @ItemList2 WHERE No = @iLoop
            
            IF @iItemType = 0
                BEGIN
                    IF @iFactor > 0 AND EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin AND ItemID = @iItemID AND Period > 0)
                        BEGIN
                            UPDATE dbo.GoodsObjectList WITH (UPDLOCK)
                                SET EndDate = DateAdd(dd, @iFactor, EndDate), Period = Period + @iFactor,
                                    @iItemUID = ItemUID, @sdtStartDate = StartDate, @sdtEndDate = EndDate
                                    WHERE OwnerLogin = @strOwnerLogin AND ItemID = @iItemID AND Period > 0
                            IF @@ERROR <> 0
                                BEGIN    GOTO FAIL_TRAN    END
                        END
                    ELSE
                        BEGIN
                            INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                                SELECT @strOwnerLogin, @strBuyerLogin, @iItemID, @sdtRegDate, @sdtNow, DateAdd(dd, @iFactor, @sdtNow), @iFactor
                            IF @@ERROR <> 0
                                BEGIN    GOTO FAIL_TRAN    END
                            SELECT @iItemUID = @@Identity, @sdtStartDate = @sdtNow, @sdtEndDate = DateAdd(dd, @iFactor, @sdtNow)
                        END
                END
            ELSE IF @iItemType = 1
                BEGIN
                    IF EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin AND GoodsID = @iItemID)
                        BEGIN
                            UPDATE dbo.DurationItemObjectList WITH (UPDLOCK)
                                SET Duration = Duration + @iFactor,
                                    @iItemUID = DurationItemUID
                                    WHERE OwnerLogin = @strOwnerLogin AND GoodsID = @iItemID
                            IF @@ERROR <> 0
                                BEGIN    GOTO FAIL_TRAN    END
                        END
                    ELSE
                        BEGIN
                            INSERT INTO dbo.DurationItemObjectList(OwnerLogin, BuyerLogin, GoodsID, Duration, RegDate)
                                SELECT @strOwnerLogin, @strBuyerLogin, @iItemID, @iFactor, @sdtRegDate
                            IF @@ERROR <> 0
                                BEGIN    GOTO FAIL_TRAN    END
                            SELECT @iItemUID = @@Identity
                        END
                END

            UPDATE @ItemList2
                SET ItemUID = @iItemUID, StartDate = @sdtStartDate, EndDate = @sdtEndDate
                    WHERE No = @iLoop

            IF @bPackageItem = 1
                BEGIN
                    UPDATE dbo.CashPackageInfoDetailList WITH (UPDLOCK)
                        SET SupplyCount = SupplyCount - CASE SupplyCount WHEN -1 THEN 0 ELSE 1 END
                            WHERE ItemID = @iOriginalItemID AND SubItemID = @iItemID
                    IF @@ERROR <> 0
                        BEGIN    GOTO FAIL_TRAN    END
                END

            SELECT @iLoop = @iLoop + 1
        END

        INSERT INTO dbo.LogCashItemList(ItemUID, OwnerLoginUID, BuyerLoginUID, Comment, BuyDate)
            SELECT ItemUID, OwnerLoginUID, BuyerLoginUID, Comment, RegDate
                FROM dbo.CashItemList(NOLOCK)
                    WHERE OwnerLoginUID = CASE @bGift_ WHEN 0 THEN BuyerLoginUID WHEN 1 THEN OwnerLoginUID END AND OwnerLoginUID = @iLoginUID_
        IF @@ERROR <> 0
            BEGIN    GOTO FAIL_TRAN    END
        
        INSERT INTO dbo.LogCashItemDetailList(ItemUID, ItemType, ItemID, Factor, PackageItem, SetItem)
            SELECT a.ItemUID, a.ItemType, a.ItemID, a.Factor, a.PackageItem, a.SetItem
                FROM dbo.CashItemDetailList as a(NOLOCK)
                    JOIN dbo.CashItemList as b(NOLOCK) ON a.ItemUID = b.ItemUID AND b.OwnerLoginUID = CASE @bGift_ WHEN 0 THEN b.BuyerLoginUID WHEN 1 THEN b.OwnerLoginUID END AND b.OwnerloginUID = @iLoginUID_
        IF @@ERROR <> 0
            BEGIN    GOTO FAIL_TRAN    END

        DELETE FROM dbo.CashItemList
            WHERE OwnerLoginUID = CASE @bGift_ WHEN 0 THEN BuyerLoginUID WHEN 1 THEN OwnerLoginUID END AND OwnerloginUID = @iLoginUID_
        IF @@ERROR <> 0
            BEGIN    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT ItemType, ItemUID, ItemID, RegDate, StartDate, EndDate, Factor
    FROM @ItemList2
        WHERE GiftItem = CASE @bGift_ WHEN 1 THEN GiftItem ELSE 0 END


