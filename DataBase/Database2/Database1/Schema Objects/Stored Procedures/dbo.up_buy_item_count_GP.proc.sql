CREATE PROCEDURE [dbo].[up_buy_item_count_GP]
	@strBuyerLogin_ [nvarchar](20),
	@strOwnerLogin_ [nvarchar](20),
	@iGoodsID_ [int],
	@iItemCount_ [int]
AS
set nocount on
set transaction isolation level read uncommitted

DECLARE     @iGP                int,
            @iPrice             int,
            @bCash              bit,
            @bShowWeb           bit,
            @iGoodsUID          int,
            @bSubSetGoods       bit,
            @iSetGoodsNum       int,
            @iNumGoods          int,
            @strGoodsUIDs       nvarchar(200),
            @strResultSQL       nvarchar(200),
            @strDurationUIDs    nvarchar(200), --godsend 리턴값을 할때 사용
            @iDuration          int,  --godsend --일반 아이템의 수량을 저장할때
            @iDurationUID       int,  --godsend  --DurationItemObjectList의 UID저장
            @iNumDurationSubSet int,  -- godsend 수량성 아이템의 셋트 갯수
            @iItemType          int

--070521. GP 변화량 기록
-- buyer의 존재, buyer의 GP를 구함
--IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strBuyerLogin_)
--BEGIN
--    SELECT -1, 0
--    RETURN
--END
--SELECT @iGP = GamePoint FROM dbo.users(NOLOCK) WHERE login = @strBuyerLogin_
declare @iLoginUID int
select  @iGP = GamePoint
,       @iLoginUID = LoginUID
    from dbo.Users
        with (nolock)
            where Login = @strBuyerLogin_
if @iLoginUID is null
    begin    select -1, 0    return    end
--070521. GP 변화량 기록

-- owner가 존재 하는지 확인
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strOwnerLogin_)
BEGIN
    SELECT -7, 0
    RETURN
END

-- 존재하는 상품?, 상품정보 알아 냄
IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_)
BEGIN
    SELECT -2, @iGP
    RETURN
END
SELECT @iPrice = Price, 
       @bCash = Cash, 
       @bShowWeb = ShowWeb, 
       @bSubSetGoods = IsSubSetGoods, 
       @iSetGoodsNum = SetGoodsNum,  
       @iDuration    = Duration --godsend
FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_

-- 상품의 정보를 확인
--IF (@bSubSetGoods = 0 AND @iSetGoodsNum = -1 )
--BEGIN
--    SELECT -10, @iGP
--    RETURN
--END

--만약 일반 아이템이라면 오류, 리턴한다.
IF @iItemType = 0
BEGIN
   SELECT -4, @iGP
    RETURN
END

--만약 구입할 아이템갯수가 0이라면 오류리턴
IF @iItemCount_ <= 0 
BEGIN
   SELECT -11, @iGP
   RETURN
END

--갯수를 카운터만큼 늘인다. 
SET @iDuration = @iDuration * @iItemCount_
IF @@ERROR <> 0
BEGIN
    SELECT -9, @iGP
    RETURN
END

-- GP로 구입할 수 있는 상품인지 확인
IF @bCash = 1
BEGIN
    SELECT -3, @iGP
    RETURN
END

-- 현재 판매 되고 있는 상품인지 확인
IF @bShowWeb = 0
BEGIN
    SELECT -8, @iGP
    RETURN
END

-- Inventory가 full인지 확인(삭제 가능)
--IF( @bSubSetGoods = 1 AND @iSetGoodsNum > -1 ) -- 세트 상품인 경우
--    SET @iNumGoods = (SELECT count(*) FROM dbo.GoodsInfoList 
--                      WHERE SetGoodsNum = @iSetGoodsNum AND GoodsID <> @iGoodsID_ AND Kind <> 3 AND Kind <> 4 ) -- 무기는 제외 시키는 조건이 추가 되어야 함
--ELSE
--    SET @iNumGoods = 1

/*
IF ((SELECT count(*) FROM dbo.GoodsObjectList WHERE OwnerLogin = @strOwnerLogin_) + @iNumGoods) > 64
BEGIN
    SELECT -4, @iGP
    RETURN
END
*/

-- 충분한 GP가 있는지 확인
SET @iPrice = @iPrice * @iItemCount_
IF @@ERROR <> 0
BEGIN
    SELECT -9, @iGP
    RETURN
END
IF @iPrice  > @iGP 
BEGIN
    SELECT -5, @iGP
    RETURN
END

-- Pattern 상품인지 확인(GoodsID에 일단위 수가 존재하면 pattern 상품)
IF (@iGoodsID_ % 10) <> 0
BEGIN
    SELECT -6, @iGP
    RETURN
END

BEGIN TRAN
    SET @strGoodsUIDs = ''
    SET @strDurationUIDs = '' --godsend 초기화
    SET @iNumGoods = 0
    SET @iNumDurationSubSet = 0 --godsend 수량성 아이템 세트 아이템의 갯수 0으로 초기화       

    IF @bSubSetGoods = 1 AND @iSetGoodsNum > -1   -- 세트 상품인 경우
    BEGIN
        -- 세트 상품을 구해 추가 해야 함
        -- 무기(Kind:3,4)는 제외 시킴
        DECLARE @iGoodsID int
        DECLARE @iDurationNum  int --godsend 셋트일때, 수량값을 저장함 
        DECLARE SetGoods_Cursor CURSOR FORWARD_ONLY FOR
        SELECT GoodsID, Duration  FROM dbo.GoodsInfoList(NOLOCK)  --godsend 수정함 
                       WHERE SetGoodsNum = @iSetGoodsNum AND 
                             GoodsID <> @iGoodsID_ AND       -- set 상품 자체를 나타내는 것 제거
                            (GoodsID % 10) = 0 AND           -- 패턴 상품은 포함 하지 않음
                             Kind <> 3 AND Kind <> 4 AND     -- 무기 포함하지 않음
                            (Kind < 10 OR Kind = 13)         -- 악세사리 포함

        OPEN SetGoods_Cursor
        FETCH NEXT FROM SetGoods_Cursor INTO @iGoodsID, @iDurationNum --godsend  수정함 
        WHILE @@FETCH_STATUS = 0
        BEGIN

            IF @iDurationNum > 0
              BEGIN
                IF NOT EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin_ AND  GoodsID = @iGoodsID )
--세트 아이템이고 수량성 아이템인데,  GoodsID를 가지고 있지 않은 경우 
                BEGIN
                  SET @strDurationUIDs = @strDurationUIDs + ', ' -- godsend
         INSERT INTO dbo.DurationItemObjectList ( OwnerLogin, BuyerLogin, GoodsID, Duration, RegDate ) 
                   VALUES ( @strOwnerLogin_, @strBuyerLogin_, @iGoodsID, @iDurationNum, GETDATE() )
                   IF @@ERROR <> 0
                       GOTO lbTran_Abort

                    -- 추가된 상품의 UID를 구함
                    SELECT @iDurationUID = @@identity
                    IF @@ERROR <> 0
                        GOTO lbTran_Abort
            
                    SET @strDurationUIDs = @strDurationUIDs + CAST(@iGoodsID AS nvarchar(10)) + ', ' + CAST(@iDurationUID AS nvarchar(10)) + ', ' +  CAST(@iDurationNum AS nvarchar(10))
                    SET @iNumDurationSubSet = @iNumDurationSubSet + 1
                  END
                  ELSE
--세트 아이템이고 수량성 아이템인데,  GoodsID를 이미 가지고 있는 경우 
                  BEGIN
                      SET @strDurationUIDs = @strDurationUIDs + ', ' -- godsend
                      DECLARE @iDurationSetItemCount int --세트 아닌 수량성 아이템중에서 이미 같은 GoodsID가 있는경우 더한 값
             SELECT @iDurationSetItemCount  = Duration FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin_ AND GoodsID = @iGoodsID
                      SET @iDurationSetItemCount = @iDurationNum + @iDurationSetItemCount

                       UPDATE dbo.DurationItemObjectList 
                       SET Duration = @iDurationSetItemCount WHERE OwnerLogin = @strOwnerLogin_  AND GoodsID = @iGoodsID 
                        IF @@ERROR <> 0
                             GOTO lbTran_Abort

                       -- 추가된 상품의 UID를 구함
                       SELECT @iDurationUID = DurationItemUID FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin_ AND GoodsID = @iGoodsID
                        IF @@ERROR <> 0
                             GOTO lbTran_Abort

                        SET @strDurationUIDs = @strDurationUIDs + CAST(@iGoodsID AS nvarchar(10)) + ', ' + CAST(@iDurationUID AS nvarchar(10)) + ', ' +  CAST(@iDurationNum AS nvarchar(10))
                        SET @iNumDurationSubSet = @iNumDurationSubSet + 1
                  END --else
              END --else
            FETCH NEXT FROM SetGoods_Cursor INTO @iGoodsID, @iDurationNum
        END -- while

        CLOSE SetGoods_Cursor
        DEALLOCATE SetGoods_Cursor
        
        IF @@ERROR <> 0
            GOTO lbTran_Abort
    END --IF 셋트일경우
    ELSE                    -- 일반 상품인 경우
     BEGIN
       IF @iDuration > 0
          BEGIN
           IF NOT EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin_ AND  GoodsID = @iGoodsID_ )
           BEGIN
--세트아닌 수량성 아이템중에서 같은 아이템이 없는 경우

               INSERT INTO dbo.DurationItemObjectList ( OwnerLogin, BuyerLogin, GoodsID, Duration, RegDate ) 
                     VALUES ( @strOwnerLogin_, @strBuyerLogin_, @iGoodsID_, @iDuration, GETDATE() )
               IF @@ERROR <> 0
                    GOTO lbTran_Abort

               SELECT @iDurationUID = @@identity
                IF @@ERROR <> 0
                     GOTO lbTran_Abort
            END
            ELSE
            BEGIN
--세트아닌 수량성 아이템중에서 이미 같은 아이템이 있는경우
                 DECLARE @iDurationItemCount int --세트 아닌 수량성 아이템중에서 이미 같은 GoodsID가 있는경우 더한 값
       SELECT @iDurationItemCount  = Duration FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin_ AND GoodsID = @iGoodsID_

                SET @iDurationItemCount = @iDuration + @iDurationItemCount

                   UPDATE dbo.DurationItemObjectList 
                   SET Duration = @iDurationItemCount  WHERE OwnerLogin = @strOwnerLogin_  AND GoodsID = @iGoodsID_ 
               IF @@ERROR <> 0
                    GOTO lbTran_Abort

               -- 추가된 상품의 UID를 구함
               SELECT @iDurationUID = DurationItemUID FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin_ AND GoodsID = @iGoodsID_
                IF @@ERROR <> 0
                     GOTO lbTran_Abort
    END --godsend

                -- 결과를 돌려 주기 위한 SELECT 문을 만듦
                SET @iNumDurationSubSet = 1
                SET @strDurationUIDs = ', ' +  CAST(@iGoodsID_ AS nvarchar(10)) + ', ' + CAST(@iDurationUID AS nvarchar(10)) + ', ' +  CAST(@iDuration  AS nvarchar(10))
          END --godsend 일반상품의 수량성 아이템
    END -- else 셋트상품이 아닌 일반상품


    -- Buyer의 GP를 변경
    --갯수만큼 가격을 깍는다. 
    SET @iGP = @iGP - @iPrice

    IF @iGP < 0 
    BEGIN
       SET @iGP = 0
    END

    UPDATE dbo.users SET GamePoint = @iGP WHERE login = @strBuyerLogin_
    IF @@ERROR <> 0
        GOTO lbTran_Abort


    -- 해당 상품 정보의 hotcount 등 수정

    UPDATE dbo.GoodsInfoList SET HotCount = HotCount + 1 WHERE GoodsID = @iGoodsID_
    IF @@ERROR <> 0
        GOTO lbTran_Abort

COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SELECT -9, @iGP
RETURN

lbTran_Success:
SET @strResultSQL = 'SELECT 0, ' +  CAST(@iGP AS nvarchar(10)) + ', ' + CAST(@iNumGoods AS nvarchar(10)) + @strGoodsUIDs + ', ' +  CAST(@iNumDurationSubSet AS nvarchar(10))  +  @strDurationUIDs
EXEC( @strResultSQL )
--PRINT @strResultSQL
RETURN


