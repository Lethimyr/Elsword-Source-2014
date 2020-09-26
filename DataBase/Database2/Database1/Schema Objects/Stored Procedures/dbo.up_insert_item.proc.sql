CREATE PROCEDURE [dbo].[up_insert_item]
	@strLoginList_ [nvarchar](4000),
	@strItemList_ [nvarchar](4000),
	@strToken_ [nvarchar](1) = N',',
	@bReturn_ [bit] = 1,
	@strIP_ [nvarchar](15) = NULL,
	@iOK [int] = 0
AS
SET NOCOUNT ON

DECLARE @LoginList TABLE
(
    No int Identity(1, 1) not null
,   Login nvarchar(20) not null
--,   primary key clustered
--    (
--        No
--    )
)

DECLARE @iStart int, @iEnd int, @strLogin nvarchar(20)
SELECT @iStart = 1, @iEnd = 1
WHILE 1 = 1
    BEGIN
        IF @strLoginList_ is NULL OR @iEnd = 4000    BEGIN   BREAK   END
        SET @iEnd = CharIndex(@strToken_, @strLoginList_, @iStart)
        IF @iEnd = 0   BEGIN   SET @iEnd = 4000   END
        SET @strLogin = LTRIM(SubString(@strLoginList_, @iStart, @iEnd - @iStart))
        SET @iStart = @iEnd + 1
        IF EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin)
            BEGIN
                INSERT INTO @LoginList(Login) VALUES(@strLogin)
            END
    END

IF NOT EXISTS(SELECT * FROM @LoginList WHERE [No] = 1)
    BEGIN    SET @iOK = -1    GOTO END_PROC    END

DECLARE @ItemList1 TABLE
(
    No int Identity(1, 1) not null
,   ItemID int not null
,   Duration int not null
--,   primary key clustered
--    (
--        No
--    )
)

DECLARE @iItemID int, @iDuration int
SELECT @iStart = 1, @iEnd = 1
WHILE 1 = 1
    BEGIN
        IF @strItemList_ is NULL OR @iEnd = 4000    BEGIN   BREAK   END
        SET @iEnd = CharIndex(@strToken_, @strItemList_, @iStart)
        IF @iEnd = 0    BEGIN   BREAK   END
        SET @iItemID = LTRIM(SubString(@strItemList_, @iStart, @iEnd - @iStart))
        SET @iStart = @iEnd + 1
        SET @iEnd = CharIndex(@strToken_, @strItemList_, @iStart)
        IF @iEnd = 0    BEGIN   SET @iEnd = 4000    END
        SET @iDuration = LTRIM(SubString(@strItemList_, @iStart, @iEnd - @iStart))
        SET @iStart = @iEnd + 1
        IF EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iItemID)
            BEGIN
                INSERT INTO @ItemList1(ItemID, Duration) VALUES(@iItemID, @iDuration)
            END
    END

IF NOT EXISTS(SELECT * FROM @ItemList1 WHERE [No] = 1)
    BEGIN    SET @iOK = -2    GOTO END_PROC    END

DECLARE @ItemList2 TABLE
(
    No int Identity(1, 1) not null
,   ItemID int not null
,   Duration int not null
,   ItemType int not null
--,   primary key clustered
--    (
--        No
--    )
)

DECLARE @iLoop1 int, @iLoopMax1 int
SELECT @iLoop1 = 1, @iLoopMax1 = MAX([No]) FROM @ItemList1
WHILE @iLoop1 <= @iLoopMax1
    BEGIN
        DECLARE @iItemType int
        SELECT @iItemID = ItemID, @iDuration = Duration FROM @ItemList1 WHERE [No] = @iLoop1

        IF EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iItemID)
        BEGIN
            DECLARE @bSetItem bit, @iSetItemCode int
            SELECT @iItemType = Itemtype, @bSetItem = IsSubSetGoods, @iSetItemCode = SetGoodsNum
                FROM dbo.GoodsInfoList(NOLOCK)
                WHERE GoodsID = @iItemID
                        
            IF @bSetItem = 1
            BEGIN
                IF @iSetItemCode = -1
                BEGIN    SET @iOK = -99    GOTO END_PROC    END
                                
                INSERT INTO @ItemList2(ItemID, Duration, ItemType)
                    SELECT GoodsID, @iDuration, @iItemType
                        FROM dbo.GoodsInfoList(NOLOCK)
                            WHERE GoodsID = ( GoodsID / 10 ) * 10
                                    AND SetGoodsNum = @iSetItemCode AND IsSubSetGoods = 0
                                    AND Kind NOT IN (3, 10, 11, 12, 14, 15, 16)
            END
            ELSE
            BEGIN
                INSERT INTO @ItemList2(ItemID, Duration, ItemType)
                    VALUES(@iItemID, @iDuration, @iItemType)
            END
        END

        SET @iLoop1 = @iLoop1 + 1
    END

IF NOT EXISTS(SELECT * FROM @ItemList2 WHERE [No] = 1)
    BEGIN    SET @iOK = -3    GOTO END_PROC    END

DECLARE @LogList TABLE
(
    [No] int Identity(1, 1) not null
,   Login nvarchar(20) not null
,   ItemType int not null
,   ItemID int not null
,   Factor int not null
,   ItemUID int not null
,   RegDate smalldatetime not null
,   StartDate smalldatetime not null
,   EndDate smalldatetime not null
--,   primary key clustered
--    (
--        No
--    )
)

DECLARE @sdtNow smalldatetime, @iLoop2 int, @iLoopMax2 int, @sdtRegDate smalldatetime, @sdtStartDate smalldatetime, @sdtEndDate smalldatetime
SELECT @sdtNow = GetDate(), @iLoop1 = 1, @iLoopMax1 = MAX(a.[No]), @iLoop2 = 1, @iLoopMax2 = MAX(b.[No])
    FROM @LoginList as a, @ItemList2 as b
SELECT @sdtRegDate = @sdtNow, @sdtStartDate = @sdtNow, @sdtEndDate = @sdtNow
BEGIN TRAN
WHILE @iLoop1 <= @iLoopMax1
    BEGIN
        SELECT @strLogin = Login FROM @LoginList WHERE [No] = @iLoop1
        WHILE @iLoop2 <= @iLoopMax2
            BEGIN
                DECLARE @iItemUID int
                SELECT @iItemUID = -1, @iItemID = ItemID, @iDuration = Duration, @iItemType = ItemType FROM @ItemList2 WHERE [No] = @iLoop2
                
                IF @iItemType = 0
                    BEGIN
                        IF EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strLogin AND ItemID = @iItemID AND Period > 0)
                            AND @iDuration > 0
                            BEGIN
                                UPDATE dbo.GoodsObjectList with (updlock)
                                    SET EndDate = DateAdd(d, @iDuration, EndDate), Period = Period + @iDuration,
                                        @iItemUID = ItemUID, @sdtRegDate = RegDate, @sdtStartDate = StartDate, @sdtEndDate = EndDate
,                                       BuyerLogin = case when @strIP_ is null then BuyerLogin else N'__' + @strIP_ + N'__' end
                                        WHERE OwnerLogin = @strLogin AND ItemID = @iItemID AND Period > 0
                                IF @@Error <> 0
                                BEGIN   SET @iOK = -10  GOTO FAIL_TRAN  END
                            END
                        ELSE
                            BEGIN
                                INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                                    VALUES(@strLogin, case when @strIP_ is null then @strLogin else N'__' + @strIP_ + N'__' end, @iItemID, @sdtNow, @sdtNow, DateAdd(d, @iDuration, @sdtNow), @iDuration)
                                IF @@Error <> 0
                                BEGIN   SET @iOK = -11  GOTO FAIL_TRAN  END

                                SELECT @iItemUID = @@Identity, @sdtEndDate = DateAdd(d, @iDuration, @sdtNow)
                            END
                    END
                ELSE
                    BEGIN
                        IF EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strLogin AND GoodsID = @iItemID)
                            BEGIN
                                UPDATE dbo.DurationItemObjectList with (updlock)
                                    SET Duration = Duration + @iDuration, @iItemUID = DurationItemUID
,                                       BuyerLogin = case when @strIP_ is null then BuyerLogin else N'__' + @strIP_ + N'__' end
                                    WHERE OwnerLogin = @strLogin AND GoodsID = @iItemID
                                IF @@Error <> 0
                                    BEGIN   SET @iOK = -12  GOTO FAIL_TRAN  END
                            END
                        ELSE
                            BEGIN
                                INSERT INTO dbo.DurationItemObjectList(OwnerLogin, BuyerLogin, GoodsID, Duration, RegDate)
                                    VALUES(@strLogin, case when @strIP_ is null then @strLogin else N'__' + @strIP_ + N'__' end, @iItemID, @iDuration, @sdtNow)
                                IF @@Error <> 0
                                    BEGIN   SET @iOK = -13  GOTO FAIL_TRAN  END

                                SET @iItemUID = @@Identity
                            END
                    END

                INSERT INTO @LogList(Login, ItemType, ItemID, Factor, ItemUID, RegDate, StartDate, EndDate)
                    VALUES(@strLogin, @iItemType, @iItemID, @iDuration, @iItemUID, @sdtRegDate, @sdtStartDate, @sdtEndDate)
                IF @@Error <> 0
                    BEGIN   SET @iOK = -20  GOTO FAIL_TRAN  END

                SET @iLoop2 = @iLoop2 + 1
            END
        SELECT @iLoop2 = 1, @iLoop1 = @iLoop1 + 1
    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
IF @bReturn_ = 1 
    BEGIN
        IF @iOK = 0 --for general purpose
            SELECT
                    @iOK
,                   [Login]
,                   ItemType
,                   ItemID
,                   Factor
,                   ItemUID
,                   RegDate
,                   StartDate
,                   EndDate
                FROM @LogList
        ELSE IF @iOK = 1 --for DBOT -_-;;;
            BEGIN
                SELECT @iOK = 0
                SELECT
                        @iOK
,                       [Login]
,                       ItemType
,                       ItemID
,                       Factor
,                       ItemUID
,                       CONVERT(nvarchar(19), RegDate, 120) as RegDate
,                       CONVERT(nvarchar(19), StartDate, 120) as StartDate
,                       CONVERT(nvarchar(19), EndDate, 120) as EndDate
                    FROM @LogList
            END
        ELSE
            SELECT @iOK --생략;
    END
ELSE
    BEGIN
        RETURN @iOK -- 호출시 exec @iOK = dbo.up_insert_item ... 식으로 받을 수 있도록 리턴한다.
    END


