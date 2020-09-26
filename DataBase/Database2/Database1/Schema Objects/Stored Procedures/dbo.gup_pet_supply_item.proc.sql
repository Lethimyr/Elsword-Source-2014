CREATE PROCEDURE [dbo].[gup_pet_supply_item]
	@iLoginUID_ [int],
	@iPetID_ [int],
	@iPromotion_ [tinyint]
AS
/*microcat
작성일 : 2007년 1월 14일
사용 : 펫 기본 아이템 지급(gup_pet_create 다음에 바로 콜이 되므로 체크는 완료)
*/
SET NOCOUNT ON;

DECLARE @sdtNow smalldatetime
SELECT  @sdtNow = GetDate()

DECLARE @SupplyItem TABLE
(
    No int not null identity(1, 1) primary key
,   ItemType tinyint not null --0:기간/무제한, 1:수량
,   ItemID int not null --세트 ItemID 불가. 무조건 파트입니다.
,   Factor int not null
,   RegDate smalldatetime not null
,   StartDate smalldatetime not null
,   EndDate smalldatetime not null
,   ItemUID int null
)

INSERT INTO @SupplyItem
    (
        ItemType
,       ItemID
,       Factor
,       RegDate
,       StartDate
,       EndDate
    )
    SELECT
            a.ItemType
,           a.ItemID
,           a.Factor
,           @sdtNow
,           @sdtNow
,           @sdtNow + CASE WHEN a.ItemType = 0 AND a.Factor > 0 THEN a.Factor ELSE 0 END
        FROM dbo.GPetItem as a(NOLOCK)
            WHERE PetID = @iPetID_ AND Promotion = @iPromotion_

IF NOT EXISTS(SELECT * FROM @SupplyItem)
    BEGIN    GOTO END_PROC    END

DECLARE @strOwnerLogin nvarchar(20)
,       @strBuyerLogin nvarchar(20)
SELECT
        @strOwnerLogin = Login
,       @strBuyerLogin = N'__PetSystem__'
    FROM dbo.Users(NOLOCK)
        WHERE LoginUID = @iLoginUID_

UPDATE a
    SET a.ItemUID = b.ItemUID
        FROM @SupplyItem as a
            JOIN dbo.GoodsObjectList as b(NOLOCK) ON b.OwnerLogin = @strOwnerLogin AND b.Period > 0 AND a.ItemID = b.ItemID AND a.ItemType = 0 AND a.Factor > 0

UPDATE a
    SET a.ItemUID = b.DurationItemUID
        FROM @SupplyItem as a
            JOIN dbo.DurationItemObjectList as b(NOLOCK) ON b.OwnerLogin = @strOwnerLogin AND a.ItemID = b.GoodsID AND a.ItemType = 1


DECLARE @iLoop int, @iLoopMax int
SELECT  @iLoop = 1
,       @iLoopMax = Max(No)
    FROM @SupplyItem
BEGIN TRAN
    UPDATE b
        SET b.EndDate = b.EndDate + a.Factor
,           b.Period = b.Period + a.Factor
            FROM @SupplyItem as a
                JOIN dbo.GoodsObjectList as b(NOLOCK) ON b.OwnerLogin = @strOwnerLogin AND b.Period > 0 AND a.ItemID = b.ItemID AND a.ItemType = 0 AND a.Factor > 0 AND a.ItemUID = b.ItemUID AND a.ItemUID IS NOT NULL
    IF @@ERROR <> 0
        BEGIN    GOTO FAIL_TRAN    END

    UPDATE b
        SET b.Duration = b.Duration + a.Factor
            FROM @SupplyItem as a
                JOIN dbo.DurationItemObjectList as b(NOLOCK) ON b.OwnerLogin = @strOwnerLogin AND a.ItemID = b.GoodsID AND a.ItemType = 1 AND a.ItemUID = b.DurationItemUID AND a.ItemUID IS NOT NULL
    IF @@ERROR <> 0
        BEGIN    GOTO FAIL_TRAN    END

    WHILE @iLoop <= @iLoopMax
        BEGIN
            DECLARE @iItemType tinyint
,                   @iItemID int
,                   @iFactor int
,                   @iItemUID int
            SELECT
                    @iItemType = ItemType
,                   @iItemID = ItemID
,                   @iFactor = Factor
,                   @iItemUID = ItemUID
                FROM @SupplyItem
                    WHERE No = @iLoop

            IF @iItemUID IS NOT NULL
                BEGIN
                    SELECT @iLoop = @iLoop + 1
                    CONTINUE
                END

            IF @iItemType = 0
                BEGIN
                    INSERT dbo.GoodsObjectList
                        (
                            OwnerLogin
,                           BuyerLogin
,                           ItemID
,                           RegDate
,                           StartDate
,                           EndDate
,                           Period
                        )
                        SELECT
                                @strOwnerLogin
,                               @strBuyerLogin
,                               @iItemID
,                               @sdtNow
,                               @sdtNow
,                               DateAdd(dd, @iFactor, @sdtNow)
,                               @iFactor
                    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                        BEGIN    GOTO FAIL_TRAN    END

                    SELECT @iItemUID = @@Identity
                END
            ELSE IF @iItemType = 1
                BEGIN
                    INSERT dbo.DurationItemObjectList
                        (
                            OwnerLogin
,                           BuyerLogin
,                           GoodsID
,                           Duration
                        )
                        SELECT
                                @strOwnerLogin
,                               @strBuyerLogin
,                               @iItemID
,                               @iFactor
                    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                        BEGIN    GOTO FAIL_TRAN    END

                    SELECT @iItemUID = @@Identity
                END

            UPDATE @SupplyItem
                SET ItemUID = @iItemUID
                    WHERE No = @iLoop
            IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                BEGIN    GOTO FAIL_TRAN    END

        SELECT @iLoop = @iLoop + 1
    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT
        ItemType
,       ItemID
,       Factor
,       RegDate
,       StartDate
,       EndDate
,       ItemUID
    FROM @SupplyItem


