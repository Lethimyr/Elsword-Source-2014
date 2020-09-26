CREATE PROCEDURE [dbo].[up_insert_item_for_cash]
	@strBuyerLogin_ [nvarchar](20),
	@strOwnerLogin_ [nvarchar](20),
	@iItemID_ [int],
	@iFactor_ [int] = -1,
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

--패키지 아이템 무제한 옵션
--판매수량 무제한 SupplyCount = 0x80000000
--판매기간 무제한 SupplyDate  = 0
--패키지 아이템 추가 방법
--insert into dbo.packageinfolist values(패키지대표ID, 패키지포함ID, 아이템타입, 펙터, 수량카운트, 날짜카운트)

declare
    @iLoginUID int
select
    @iLoginUID = LoginUID
from dbo.Users
where
    Login = @strOwnerLogin_

if @iLoginUID is null
begin    select @iOK = -1    goto end_proc    end

declare
    @iPrice int
,   @iItemType int
,   @bSubSetGoods bit
,   @iSetGoodsNum int
,   @iFactor int
select
    @iPrice = Price
,   @iItemType = ItemType
,   @bSubSetGoods = IsSubSetGoods
,   @iSetGoodsNum = SetGoodsNum
,   @iFactor = Duration
from dbo.GoodsInfoList
where
    GoodsID = @iItemID_
and Cash = 1
and ShowWeb = 1
and GoodsID % 10 = 0

if @iItemType is null
begin    select @iOK = -2    goto end_proc    end

IF @iItemType = 0
begin
    declare
        @iPriceTemp int

    select
        @iPriceTemp = Price
    from dbo.ItemPriceByPeriod
    where
        ItemID = @iItemID_
    and Period = @iFactor_

    if @iPriceTemp is null
    begin    select @iOK = -3    goto end_proc    end

    select
        @iPrice = @iPriceTemp
end

DECLARE @ItemList TABLE
(
    No int not null identity(1, 1) primary key,
    ItemID int not null,
    ItemType tinyint not null,
    Factor int not null
)

IF EXISTS(SELECT * FROM dbo.PackageInfoList(NOLOCK) WHERE ItemID = @iItemID_)
    BEGIN
        INSERT INTO @ItemList(ItemID, ItemType, Factor)
            SELECT PackageItemID, ItemType, Factor FROM dbo.PackageInfoList(NOLOCK)
                WHERE ItemID = @iItemID_ AND (SupplyCount > 0 OR SupplyCount = 0x80000000) AND (SupplyDate > GetDate() OR SupplyDate = 0)
    END
ELSE IF @iItemType = 0 AND @bSubSetGoods = 1
    BEGIN
        INSERT INTO @ItemList(ItemID, ItemType, Factor)
            SELECT GoodsID, ItemType, @iFactor_ FROM dbo.GoodsInfoList(NOLOCK)
                WHERE SetGoodsNum = @iSetGoodsNum AND 
                        GoodsID <> @iItemID_ AND 
                        (GoodsID % 10) = 0 AND 
                        Kind <> 3 AND Kind <> 4 AND
                        (Kind < 10 OR Kind = 13)
    END
ELSE
    BEGIN
        IF @iItemType = 1
            BEGIN
                SELECT @iFactor_ = @iFactor
            END
        INSERT INTO @ItemList(ItemID, ItemType, Factor)
            SELECT @iItemID_, @iItemType, @iFactor_
    END

IF NOT EXISTS(SELECT * FROM @ItemList)
    BEGIN    SELECT @iOK = -4    GOTO END_PROC    END


DECLARE @iLoop int, @iLoopMax int, @sdtNow smalldatetime
SELECT @iLoop = 1, @iLoopMax = MAX(No), @sdtNow = GetDate() FROM @ItemList
begin transaction
    WHILE @iLoop <= @iLoopMax
        BEGIN
            DECLARE @iItemID int
            SELECT @iItemID = ItemID, @iItemType = ItemType, @iFactor = Factor FROM @ItemList WHERE No = @iLoop

            IF @iItemType = 0
                BEGIN
                    IF @iFactor > 0
                        BEGIN
                            IF EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE Ownerlogin = @strOwnerLogin_ AND ItemID = @iItemID AND Period > 0)
                                BEGIN
                                    UPDATE dbo.GoodsObjectList WITH (UPDLOCK)
                                        SET EndDate = EndDate + @iFactor, Period = Period + @iFactor
                                            WHERE Ownerlogin = @strOwnerLogin_ AND ItemID = @iItemID AND Period > 0
                                    IF @@ERROR <> 0
                                        BEGIN    SELECT @iOK = -10    GOTO FAIL_TRAN    END
                                END
                            ELSE
                                BEGIN
                                    INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                                        VALUES(@strOwnerLogin_, @strBuyerLogin_, @iItemID, @sdtNow, @sdtNow - 1, @sdtNow - 1, @iFactor)
                                    IF @@ERROR <> 0
                                        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END
                                END
                        END
                    ELSE
                        BEGIN
                            INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                                VALUES(@strOwnerLogin_, @strBuyerLogin_, @iItemID, @sdtNow, @sdtNow, @sdtNow -1, -1)
                            IF @@ERROR <> 0
                                BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
                        END
                END
            ELSE IF @iItemType = 1
                BEGIN
                    IF EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE Ownerlogin = @strOwnerLogin_ AND GoodsID = @iItemID)
                        BEGIN
                            UPDATE dbo.DurationItemObjectList WITH (UPDLOCK)
                                SET Duration = Duration + @iFactor
                                    WHERE Ownerlogin = @strOwnerLogin_ AND GoodsID = @iItemID
                            IF @@ERROR <> 0
                                BEGIN    SELECT @iOK = -13    GOTO FAIL_TRAN    END
                        END
                    ELSE
                        BEGIN
                            INSERT INTO dbo.DurationItemObjectList(OwnerLogin, BuyerLogin, GoodsID, Duration)
                                VALUES(@strOwnerLogin_, @strBuyerLogin_, @iItemID, @iFactor)
                            IF @@ERROR <> 0
                                BEGIN    SELECT @iOK = -14    GOTO FAIL_TRAN    END
                        END
                END

            DECLARE @iSupplyCount int
            SELECT @iSupplyCount = 0x80000000
            SELECT @iSupplyCount = SupplyCount FROM dbo.PackageInfoList(NOLOCK)
                WHERE ItemID = @iItemID_ AND PackageItemID = @iItemID AND ItemType = @iItemType AND Factor = @iFactor
            IF @iSupplyCount > 0
                BEGIN
                    UPDATE dbo.PackageInfoList WITH (ROWLOCK)
                        SET SupplyCount = @iSupplyCount - 1
                            WHERE ItemID = @iItemID_ AND PackageItemID = @iItemID AND ItemType = @iItemType AND Factor = @iFactor
                    IF @@ERROR <> 0
                        BEGIN    SELECT @iOK = -15    GOTO FAIL_TRAN    END
                END            
            
            SELECT @iLoop = @iLoop + 1
        END

insert into [Log].dbo.ICLOItemCashItem
with (updlock)
(
    LoginUID
,   RegDateA
,   BuyerLogin
,   ItemID
,   Factor
,   Price
)
select
    @iLoginUID
,   current_timestamp
,   @strBuyerLogin_
,   @iItemID_
,   @iFactor_
,   @iPrice

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
return
    @iOK


