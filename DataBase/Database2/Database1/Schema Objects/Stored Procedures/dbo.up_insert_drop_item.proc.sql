CREATE PROCEDURE [dbo].[up_insert_drop_item]
	@strLogin_ [nvarchar](20),
	@iGoodsID_ [int],
	@iDuration_ [int]
AS
/*
- 기능: 떨어진 아이템 넣어주기, 세트는 불가
- In:   @strLogin_, @iGoodsID_, @iDuration_
- Out:  @iOK, NumNormal = 1, GoodsID, GoodsUID, RegDate, StartDate, EndDate, Period, NumDuration = 0
        @iOK, NumNormal = 0, NumDuration = 1, GoodsID, GoodsUID, Duration
- iOK:  0( 성공 )
       -1( login이 존재 하지 않음 ) 
       -2( 해당 상품이 존재 하지 않음 )
       -6( pattern 상품인 경우 구입 할 수 없음, up_buy_pattern sp 참조 )
       -7( 세트 상품은 구입 불가 )
       -9( 알수 없는 error )

exec dbo.up_insert_drop_item 'microcat', 200, -1
exec dbo.up_insert_drop_item 'microcat', 12490, 1
*/

SET NOCOUNT ON;

DECLARE @iOK int,
        @strSQL nvarchar(4000),
        @strSQLP nvarchar(4000),
        @iItemType int,
--        @iCharType int,
        @bSubSetGoods bit,
        @iSetGoodsNum int,
        @iGoodsUID int

SELECT @iOK = 0,
       @strSQL = N'SELECT @iOK',
       @strSQLP = N'@iOK int, @iNormalItem int, @iDurationItem int, @iGoodsID_ int, @iGoodsUID int, @iDuration_ int, @sdtRegDate smalldatetime, @sdtStartDate smalldatetime, @sdtEndDate smalldatetime'

--조건 체크 시작
IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_)
    BEGIN   SET @iOK = -1   GOTO END_PROC   END

IF @iGoodsID_ % 10 <> 0
    BEGIN   SET @iOK = -6   GOTO END_PROC   END

IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_)
    BEGIN   SET @iOK = -2   GOTO END_PROC   END

SELECT @iItemType = ItemType,
--       @iCharType = CharType,
       @bSubSetGoods = IsSubSetGoods,
       @iSetGoodsNum = SetGoodsNum
    FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_

IF @bSubSetGoods = 1 AND @iSetGoodsNum > -1
    BEGIN   SET @iOK = -7   GOTO END_PROC   END

--IF @iCharType <> -1
--BEGIN
--    IF NOT EXISTS(SELECT * FROM dbo.Characters(NOLOCK) WHERE Login = @strLogin_ AND CharType = @iCharType)
--        BEGIN   SET @iOK = -8   GOTO END_PROC   END
--END
--조건 체크 끝

--데이터 조작 시작
DECLARE @sdtNow smalldatetime,
        @iNormalItem int,
        @iDurationItem int,
        @sdtRegDate smalldatetime,
        @sdtStartDate smalldatetime,
        @sdtEndDate smalldatetime

SELECT @sdtNow = GetDate(), @iNormalItem = 0, @iDurationItem = 0,
       @sdtRegDate = @sdtNow, @sdtStartDate = @sdtNow, @sdtEndDate = @sdtNow
--데이터 조작 끝
--070116. microcat. 수량이 음수로 들어올 경우 에러처리~
IF @iDuration_ < 0 AND @iItemType = 1
    BEGIN   SET @iOK = -9   GOTO END_PROC   END
--DB 작업 시작
BEGIN TRAN
    IF @iItemType = 0
        BEGIN
            IF EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND ItemID = @iGoodsID_ AND Period > 0)
                AND @iDuration_ > 0
                BEGIN
                    UPDATE dbo.GoodsObjectList
                        SET EndDate = DateAdd(d, @iDuration_, EndDate), Period = Period + @iDuration_
                            WHERE OwnerLogin = @strLogin_ AND ItemID = @iGoodsID_ AND Period > 0
                    IF @@Error <> 0
                    BEGIN   SET @iOK = -10  GOTO FAIL_TRAN  END
                    SELECT @iGoodsUID = ItemUID, @sdtRegDate = RegDate, @sdtStartDate = StartDate, @sdtEndDate = EndDate, @iDuration_ = Period
                        FROM dbo.GoodsObjectList(NOLOCK)
                            WHERE OwnerLogin = @strLogin_ AND ItemID = @iGoodsID_ AND Period > 0
                END
            ELSE
                BEGIN
                    INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                        VALUES(@strLogin_, @strLogin_, @iGoodsID_, @sdtNow, @sdtNow, DateAdd(d, @iDuration_, @sdtNow), @iDuration_)
                    IF @@ERROR <> 0 BEGIN   SET @iOK = -9   GOTO FAIL_TRAN  END
                    SELECT @iGoodsUID = @@Identity, @sdtEndDate = DateAdd(d, @iDuration_, @sdtNow)
                END

            SELECT @iNormalItem = 1,
                   @strSQL = @strSQL + ', @iNormalItem, @iGoodsID_, @iGoodsUID, @sdtRegDate, @sdtStartDate, @sdtEndDate, @iDuration_, @iDurationItem'
        END
    ELSE
        BEGIN
            IF NOT EXISTS(SELECT * from dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND GoodsID = @iGoodsID_)
                BEGIN
                    INSERT INTO dbo.DurationItemObjectList (OwnerLogin, BuyerLogin, GoodsID, Duration, RegDate) 
                        VALUES(@strLogin_, @strLogin_, @iGoodsID_, @iDuration_, @sdtNow)
                    IF @@ERROR <> 0 BEGIN   SET @iOK = -9   GOTO FAIL_TRAN  END
                    SELECT @iGoodsUID = @@Identity
                END
            ELSE
                BEGIN
                    UPDATE dbo.DurationItemObjectList WITH (UPDLOCK)
                        SET Duration = Duration + @iDuration_, @iGoodsUID = DurationitemUID
                            WHERE OwnerLogin = @strLogin_ AND GoodsID = @iGoodsID_
                    IF @@ERROR <> 0 BEGIN   SET @iOK = -9   GOTO FAIL_TRAN  END
                END
            SELECT @iDurationItem = 1,
                   @strSQL = @strSQL + ', @iNormalItem, @iDurationItem, @iGoodsID_, @iGoodsUID, @iDuration_'
        END
COMMIT TRAN
--DB 작업 끝

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
exec dbo.sp_executesql @strSQL, @strSQLP, @iOK, @iNormalItem, @iDurationItem, @iGoodsID_, @iGoodsUID, @iDuration_, @sdtRegDate, @sdtStartDate, @sdtEndDate


