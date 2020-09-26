CREATE PROCEDURE [dbo].[up_drop_item]
	@strLogin_ [nvarchar](20),
	@iGoodsID_ [int],
	@iGoodsUID_ [int],
	@bExpired_ [bit],
	@bPreExpired_ [bit] = 0,
	@bPureDelete_ [bit] = 0
AS
set nocount on
set transaction isolation level read uncommitted

DECLARE @iOK                 int,
        @iGP                 int,
        @iReturnGP           int,
        @bDuration           bit,
        @iGemGoodsID         int,
        @iGemGoodsUID        int, 
        @iAddGem             int, 
        @iCrystalGoodsID     int,
        @iCrystalGoodsUID    int,
        @iAddCrystal         int,
        @fCashPerGem         float, --잼 한개당 캐쉬량
        @fDivideFactor       float, --원래 가격에서 얼만큼 나눌 것임?
        @iGoodsIDX           int

SELECT @iOK = 0, @iGP = 0, @iReturnGP = 0, @bDuration = 0,
       @iGemGoodsID = 8880, @iGemGoodsUID = 0, @iAddGem = 0,
       @iCrystalGoodsID = 12490, @iCrystalGoodsUID = 0, @iAddCrystal = 0,
       @fCashPerGem = Variable2, @fDivideFactor = Variable1, @iGoodsIDX = @iGoodsID_
    FROM dbo.GlobalVariables(NOLOCK) WHERE [No] = 4

--070521. GP 변화량 기록
--IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strLogin_)
--    BEGIN    SET @iOK = -1    GOTO lbTran_Success    END
--
--SELECT @iGP = GamePoint FROM dbo.users(NOLOCK) WHERE login = @strLogin_
declare @iLoginUID int
select  @iGP = GamePoint
,       @iLoginUID = LoginUID
    from dbo.Users
        with (nolock)
            where Login = @strLogin_
if @iLoginUID is null
    begin    select @iOK = -1    goto lbTran_Success    end
--070521. GP 변화량 기록


IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_)
    BEGIN    SET @iOK = -2    GOTO lbTran_Success    END

DECLARE @iDuration           int,
        @iItemType           int,
        @iPrice              int,
        @bCash               bit,
        @iGem                int,
        @iCrystal            int

IF (@iGoodsIDX % 10) > 0 --패턴과 기본 아이템은 아이템 타입이 같다고 가정함.
    BEGIN
        SET @iGoodsIDX = @iGoodsIDX / 10 * 10
    END

SELECT @iDuration = Duration,
       @iItemType = ItemType,
       @iPrice = Price,
       @bCash = Cash,
       @iGem = Gem,
       @iCrystal = Crystal
    FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsIDX

DECLARE @strOwner            nvarchar(20),
        @strBuyer            nvarchar(20),
        @iGoodsID            int,
        @regDate             smalldatetime,
        @StartDate           smalldatetime,
        @EndDate             smalldatetime,
        @iPeriod             int,
        @iUnusedDuration     int

SELECT @strOwner = '', @iGoodsID = -1
IF @iItemType = 0
    BEGIN
        IF NOT EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE ItemUID = @iGoodsUID_ AND OwnerLogin = @strLogin_ AND ItemID = @iGoodsID_)
            BEGIN    SET @iOk = -3    GOTO lbTran_Success    END

        SELECT @strOwner = OwnerLogin,
               @strBuyer = BuyerLogin,
               @iGoodsID = ItemID,
               @regDate = RegDate,
               @StartDate = StartDate,
               @EndDate = EndDate,
               @iPeriod = Period
            FROM dbo.GoodsObjectList(NOLOCK) WHERE ItemUID = @iGoodsUID_

        IF @iDuration <> -1 --만약 일반 아이템이고 기간제 아이템이라면 가격을 받아올 필요가 없다.
            BEGIN
                SELECT @iPrice = Price FROM dbo.ItemPriceByPeriod(NOLOCK)
                    WHERE ItemID = @iGoodsIDX AND Period = -1
            END
    END
ELSE
    BEGIN
        IF NOT EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE DurationItemUID = @iGoodsUID_ AND OwnerLogin = @strLogin_ AND GoodsID = @iGoodsID_)
            BEGIN    SET @iOk = -4    GOTO lbTran_Success    END

        SELECT @strOwner = OwnerLogin,
               @strBuyer = BuyerLogin,
               @iGoodsID = GoodsID,
               @regDate = RegDate,
               @iUnusedDuration = Duration
            FROM dbo.DurationItemObjectList(NOLOCK) WHERE DurationItemUID = @iGoodsUID_
    END

IF @strLogin_ <> @strOwner
    BEGIN    SET @iOk = -5    GOTO lbTran_Success    END

IF @iGoodsID <> @iGoodsID_
    BEGIN    SET @iOK = -6    GOTO lbTran_Success    END

DECLARE @iAdd int, @fBuyPrice float, @fFactor float, @fUnusedPercent float, @iAddType int

SELECT @fBuyPrice =
                    CASE
                        WHEN @iCrystal > 0 THEN @iCrystal --크리스탈
                        WHEN @iGem > 0 THEN @iGem --잼
                        ELSE @iPrice --GP, 캐쉬
                    END,
       @fFactor =
                    CASE
                        WHEN @bCash = 1 THEN @fCashPerGem --캐시
                        ELSE @fDivideFactor --GP, 잼, 크리스탈
                    END,
       @fUnusedPercent =
                    CASE
                        WHEN @iItemType = 1 THEN CAST(@iUnusedDuration as float) / CAST(@iDuration as float) --수량
                        WHEN @iPeriod <> -1 THEN 0 --기간제
                        ELSE 1 --기타...(-_-;;;)
                    END,
       @iAddType =
                    CASE
                        WHEN @iPeriod <> -1 THEN 3 --아무 것도 안줘야지
                        WHEN @iCrystal > 0 THEN 0 --크리스탈
                        WHEN @iGem > 0 OR @bCash = 1 THEN 1 --잼
                        ELSE 2 --GP
                    END

BEGIN TRAN
    IF @iItemType = 0
        BEGIN
            SET @bDuration = 0;
            DELETE FROM dbo.GoodsObjectList WHERE ItemUID = @iGoodsUID_
            IF @@ERROR <> 0     BEGIN    SET @iOK = -100    GOTO lbTran_Abort    END

            insert into [Log].dbo.IPLOItemPeriodItem
            with (updlock)
            (
                LoginUID
            ,   ItemUID
            ,   BuyerLogin
            ,   ItemID
            ,   RegDate
            ,   DelDate
            ,   StartDate
            ,   EndDate
            ,   Period
            ,   Expired
            )
            select
                @iLoginUID
            ,   @iGoodsUID_
            ,   @strBuyer
            ,   @iGoodsID_
            ,   @regDate
            ,   current_timestamp
            ,   @StartDate
            ,   @EndDate
            ,   @iPeriod
            ,   @bExpired_

            IF @@ERROR <> 0
            BEGIN    select @iOK = -101    GOTO lbTran_Abort    END
        END
    ELSE IF @iItemType = 1
        BEGIN
            SET @bDuration = 1;
            DELETE  FROM dbo.DurationItemObjectList WHERE DurationItemUID =  @iGoodsUID_
            IF @@ERROR <> 0     BEGIN    SET @iOK = -102    GOTO lbTran_Abort    END

            insert into [Log].dbo.ICLOItemCountItem
            with (updlock)
            (
                LoginUID
            ,   ItemUID
            ,   BuyerLogin
            ,   ItemID
            ,   RegDate
            ,   DelDate
            )
            select
                @iLoginUID
            ,   @iGoodsUID_
            ,   @strBuyer
            ,   @iGoodsID_
            ,   @regDate
            ,   current_timestamp

            IF @@ERROR <> 0
            BEGIN    select @iOK = -103    GOTO lbTran_Abort    END
        END
    ELSE
        BEGIN
            GOTO lbTran_Abort
        END

    SET @iReturnGP = @iGP

    IF @bPureDelete_ = 0
        BEGIN
            IF @iAddType = 0 --크리스탈 아이템을 팔았다면 크리스탈로...
                BEGIN
                    SELECT @iAdd = (@fBuyPrice / @fFactor) * (@fUnusedPercent), @iAddCrystal = @iAdd
                    IF NOT EXISTS( SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwner AND GoodsID = @iCrystalGoodsID )
                        BEGIN
                            INSERT INTO dbo.DurationItemObjectList ( OwnerLogin, BuyerLogin, GoodsID, Duration ) 
                                VALUES ( @strOwner, @strOwner, @iCrystalGoodsID, @iAddCrystal )
                            IF @@ERROR <> 0     BEGIN    SET @iOK = -104    GOTO lbTran_Abort    END
                            SELECT @iCrystalGoodsUID = @@identity
                        END
                    ELSE
                        BEGIN
                            UPDATE dbo.DurationItemObjectList WITH (UPDLOCK)
                                SET Duration = Duration + @iAddCrystal, @iCrystalGoodsUID = DurationItemUID
                                    WHERE OwnerLogin = @strOwner AND GoodsID = @iCrystalGoodsID
                            IF @@ERROR <> 0     BEGIN    SET @iOK = -105    GOTO lbTran_Abort    END
                        END
                END
            ELSE IF @iAddType = 1 --잼 아이템이나 캐쉬 아이템을 팔았다면 잼으로...
                BEGIN
   SELECT @iAdd = (@fBuyPrice / @fFactor) * (@fUnusedPercent), @iAddGem = @iAdd
                    IF NOT EXISTS( SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwner AND GoodsID = @iGemGoodsID )
                        BEGIN
                            INSERT INTO dbo.DurationItemObjectList ( OwnerLogin, BuyerLogin, GoodsID, Duration ) 
                                VALUES ( @strOwner, @strOwner, @iGemGoodsID, @iAddGem )
                            IF @@ERROR <> 0     BEGIN    SET @iOK = -106    GOTO lbTran_Abort    END
                            SELECT @iGemGoodsUID = @@identity
                        END
                    ELSE
                        BEGIN
                            UPDATE dbo.DurationItemObjectList WITH (UPDLOCK)
                                SET Duration = Duration + @iAddGem, @iGemGoodsUID = DurationItemUID
                                    WHERE OwnerLogin = @strOwner AND GoodsID = @iGemGoodsID
                            IF @@ERROR <> 0     BEGIN    SET @iOK = -107    GOTO lbTran_Abort    END
                        END
                END
            ELSE IF @iAddType = 2 --GP 아이템은 GP로...
                BEGIN
                    SELECT @iAdd = (@fBuyPrice / @fFactor) * (@fUnusedPercent), @iReturnGP = dbo.zcGamePoint(@iAdd + @iReturnGP)
                    UPDATE dbo.users SET GamePoint = @iReturnGP WHERE login = @strOwner
                    IF @@ERROR <> 0     BEGIN    SET @iOK = -108    GOTO lbTran_Abort    END
                END
        END
COMMIT TRAN

GOTO lbTran_Success

lbTran_Abort: --여기 지나가면 좀 심각하다...
ROLLBACK TRAN
SELECT @iReturnGP = @iGP, @bDuration = 0,
       @iGemGoodsID = 8880, @iGemGoodsUID = 0, @iAddGem = 0,
       @iCrystalGoodsID = 12490, @iCrystalGoodsUID = 0, @iAddCrystal = 0

lbTran_Success:
IF @bPreExpired_ = 0
    SELECT @iOK, @iReturnGP, @bDuration, @iGemGoodsID, @iGemGoodsUID, @iAddGem, @iCrystalGoodsID, @iCrystalGoodsUID, @iAddCrystal


