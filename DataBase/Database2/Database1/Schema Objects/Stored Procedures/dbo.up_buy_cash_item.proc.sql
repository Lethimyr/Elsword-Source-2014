CREATE PROCEDURE [dbo].[up_buy_cash_item]
	@iOwnerLoginUID_ [int],
	@ItemList_ [nvarchar](4000),
	@iBuyerLoginUID_ [int],
	@strComment_ [nvarchar](80) = '',
	@bBuyerCheck_ [bit] = 0,
	@iHaveMoney_ [int] = -1,
	@bFromWeb_ [bit] = 0,
	@iOK [int] = 0
AS
SET NOCOUNT ON;

IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE LoginUID = @iOwnerLoginUID_)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

IF @bBuyerCheck_ = 1
    BEGIN
        IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE LoginUID = @iBuyerLoginUID_)
            BEGIN    SELECT @iOK = -2    GOTO END_PROC    END
    END

DECLARE @ItemList TABLE
(
    No int not null identity(1, 1) primary key,
    ItemType int not null,
    ItemID int not null,
    Factor int not null,
    Price int not null,
    PackageItem bit not null,
    SetItem bit not null
)

DECLARE @iStart int, @iEnd int, @iItemID int, @iFactor int, @iTotalPrice int, @sdtNow smalldatetime
SELECT @iStart = 0, @iEnd = 1, @iTotalPrice = 0, @sdtNow = GetDate()
WHILE 1 = 1
    BEGIN
        IF @ItemList_ is NULL OR @iEnd = 4000    BEGIN   BREAK   END
        SELECT @iEnd = CharIndex(',', @ItemList_, @iStart)
        IF @iEnd = 0    BEGIN   BREAK   END
        SELECT @iItemID = LTRIM(SubString(@ItemList_, @iStart, @iEnd - @iStart))
        SELECT @iStart = @iEnd + 1
        SELECT @iEnd = CharIndex(',', @ItemList_, @iStart)
        IF @iEnd = 0    BEGIN   SET @iEnd = 4000    END
        SELECT @iFactor = LTRIM(SubString(@ItemList_, @iStart, @iEnd - @iStart))
        SELECT @iStart = @iEnd + 1

        DECLARE @bCash bit, @bShowWeb bit, @bSetItem bit, @iItemType int, @iFactorOriginal int, @iPrice int
        SELECT @bCash = 0, @bShowWeb = 0, @bSetItem = 0, @iItemType = -1, @iFactorOriginal = 0, @iPrice = -1
        
        SELECT @bCash = Cash, 
               @bShowWeb = ShowWeb, 
               @bSetItem = IsSubSetGoods,
               @iItemType = ItemType,
               @iFactorOriginal = Duration,
               @iPrice = Price
            FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iItemID

        IF @bCash = 0 OR @bShowWeb = 0 OR @iItemType = -1 OR @iItemID % 10 <> 0 OR @iPrice = -1
            BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

        IF @iItemType = 0
            BEGIN
                DECLARE @iPricePeriod int
                SELECT @iPricePeriod = -1
                SELECT @iPricePeriod = Price FROM dbo.ItemPriceByPeriod(NOLOCK) WHERE ItemID = @iItemID AND Period = @iFactor
                IF @iPricePeriod = -1
                    BEGIN    SELECT @iOK = -4    GOTO END_PROC    END
                SELECT @iPrice = @iPricePeriod
            END
        ELSE IF @iItemType = 1
            BEGIN
                SELECT @iFactor = @iFactorOriginal
            END
        SELECT @iTotalPrice = @iTotalPrice + @iPrice

        DECLARE @iSupplyCount int, @sdtSupplyDate smalldatetime, @bPackageItem bit
        SELECT @iSupplyCount = -2, @sdtSupplyDate = 0, @bPackageItem = 0
        SELECT @iSupplyCount = SupplyCount, @sdtSupplyDate = SupplyDate FROM dbo.CashPackageInfoList(NOLOCK) WHERE ItemID = @iItemID
        IF @iSupplyCount <> -2
            BEGIN
                IF @iSupplyCount = 0 OR (@sdtSupplyDate <= @sdtNow AND @sdtSupplyDate <> 0)
                    BEGIN
                        BEGIN TRAN
                            UPDATE dbo.GoodsInfoList WITH (ROWLOCK) SET ShowWeb = 0 WHERE GoodsID = @iItemID AND Cash = 1
                            IF @@ERROR <> 0
                                BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END
                    END
                SELECT @bPackageItem = 1
            END

        INSERT INTO @ItemList(ItemType, ItemID, Factor, Price, PackageItem, SetItem)
            SELECT @iItemType, @iItemID, @iFactor, @iPrice, @bPackageItem, @bSetItem
    END

IF NOT EXISTS(SELECT * FROM @ItemList)
    BEGIN    SELECT @iOK = -5    GOTO END_PROC    END

IF @iHaveMoney_ < @iTotalPrice AND @iHaveMoney_ >= 0
    BEGIN    SELECT @iOK = -6    GOTO END_PROC    END

DECLARE @iLoop int, @iLoopMax int, @iItemUID int
SELECT @iLoop = 1, @iLoopMax = MAX(No) FROM @ItemList

BEGIN TRAN
    INSERT INTO dbo.CashItemList(OwnerLoginUID, BuyerLoginUID, Comment)
        SELECT @iOwnerLoginUID_, @iBuyerLoginUID_, @strComment_
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
    SELECT @iItemUID = @@Identity

    INSERT INTO dbo.CashItemDetailList(ItemUID, ItemType, ItemID, Factor, PackageItem, SetItem)
        SELECT @iItemUID, ItemType, ItemID, Factor, PackageItem, SetItem
            FROM @ItemList
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -13    GOTO FAIL_TRAN    END

    INSERT INTO dbo.CashItemDisplayList(ItemUID, BuyerLoginUID, OwnerLoginUID, ItemID, Factor, Price, FromWeb, RegDate)
        SELECT @iItemUID, @iBuyerLoginUID_, @iOwnerLoginUID_, ItemID, Factor, Price, @bFromWeb_, @sdtNow
            FROM @ItemList
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -14    GOTO FAIL_TRAN    END

    UPDATE a WITH (UPDLOCK)
        SET a.SupplyCount = a.SupplyCount - CASE a.SupplyCount WHEN -1 THEN 0 ELSE 1 END
        FROM dbo.CashPackageInfoList as a JOIN @ItemList as b
            ON a.ItemID = b.ItemID AND b.PackageItem = 1
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -15    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
IF @iOK <> 0
    BEGIN
        SELECT @iItemUID = 0, @iTotalPrice = 0
    END
SELECT @iOK, @iItemUID, @iTotalPrice, ''


