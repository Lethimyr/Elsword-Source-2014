CREATE PROCEDURE [dbo].[up_buy_crystal_item]
	@strBuyerLogin_ [nvarchar](20),
	@strOwnerLogin_ [nvarchar](20),
	@iGoodsID_ [int],
	@iDisCountPercent [int],
	@iDuration_ [int]
AS
exec dbo.up_log_set_proc_count 'up_buy_crystal_item'

SET NOCOUNT ON

DECLARE @iCrystal                       int,   --구입에 필요한 Crystal갯수
        @iMyCrystalUID	                int,   --내가 가진 Crystal의 UID
        @iCrystalGoodsID                int, -- Crystal의 goodsid 
        @iMyCrystal                     int, -- 내가 가진  Crystal갯수 
        @bCash                          bit,
        @bShowWeb                       bit,
        @iGoodsUID                      int,
        @bSubSetGoods                   bit,
        @iSetGoodsNum                   int,
        @iNumGoods                      int,
        @strGoodsUIDs                   nvarchar(1024),
        @strResultSQL                   nvarchar(1024),
        @strDurationUIDs                nvarchar(1024), --godsend 리턴값을 할때 사용
        @iDuration	                    int,  --godsend --일반 아이템의 수량을 저장할때
        @iDurationUID                   int,  --godsend  --DurationItemObjectList의 UID저장
        @iNumDurationSubSet             int,  -- godsend 수량성 아이템의 셋트 갯수
        @iCrystalItemDiscountPersent    int,
        @iItemType                      int

--CrystalGoodsID 하드코딩
SET @iCrystalGoodsID = 12490
-- buyer의 존재, buyer의 GP를 구함
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strBuyerLogin_)
BEGIN
    SELECT -1, 0
    RETURN
END

IF NOT EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE Ownerlogin = @strBuyerLogin_ AND GoodsID = @iCrystalGoodsID)
BEGIN
    SELECT -12, 0
    RETURN
END
SELECT @iMyCrystal  = Duration, @iMyCrystalUID = DurationItemUID FROM dbo.DurationItemObjectList(NOLOCK) WHERE Ownerlogin = @strBuyerLogin_ AND GoodsID = @iCrystalGoodsID

-- owner가 존재 하는지 확인
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strOwnerLogin_)
BEGIN
    SELECT -7, 0
    RETURN
END

-- 존재하는 상품?, 상품정보 알아 냄
IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_)
BEGIN
    SELECT -2, @iMyCrystal
    RETURN
END
SELECT @bCash = Cash, 
       @bShowWeb = ShowWeb, 
       @bSubSetGoods = IsSubSetGoods, 
       @iSetGoodsNum = SetGoodsNum,  
       @iDuration = Duration, --godsend
       @iCrystal = Crystal,  --구입에 필요한 Crystal의 갯수 
       @iItemType = ItemType
    FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_

-- 상품의 정보를 확인
--IF (@bSubSetGoods = 0 AND @iSetGoodsNum = -1 )
--BEGIN
--    SELECT -10, @iGP
--    RETURN
--END

-- GP로 구입할 수 있는 상품인지 확인
IF @bCash = 1
BEGIN
    SELECT -3, @iMyCrystal
    RETURN
END

--Crystal으로 구입할수 있는 상품인지 확인
IF @iCrystal <= 0 
BEGIN
    SELECT -11, @iMyCrystal
    RETURN
END

-- 현재 판매 되고 있는 상품인지 확인
IF @bShowWeb = 0
BEGIN
    SELECT -8, @iMyCrystal
    RETURN
END

--크리스탈 할인율을 적용한다. 
IF @iDisCountPercent > 0
BEGIN
SET @iCrystalItemDiscountPersent = 100 - @iDisCountPercent
SET @iCrystal = CAST(@iCrystal as float) * CAST(@iCrystalItemDiscountPersent as float)  / 100.0
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

-- 충분한 Crystal이 있는지 확인
IF @iCrystal > @iMyCrystal
BEGIN
    SELECT -5, @iMyCrystal
    RETURN
END

-- Pattern 상품인지 확인(GoodsID에 일단위 수가 존재하면 pattern 상품)
IF (@iGoodsID_ % 10) <> 0
BEGIN
    SELECT -6, @iMyCrystal
    RETURN
END

BEGIN TRAN
    SET @strGoodsUIDs = ''
    SET @strDurationUIDs = '' --godsend 초기화
    SET @iNumGoods = 0
    SET @iNumDurationSubSet = 0 --godsend 수량성 아이템 세트 아이템의 갯수 0으로 초기화       

    DECLARE @sdtNow smalldatetime
    SET @sdtNow = GetDate()
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
            IF @iItemType = 0  --셋트중에서 일반 아이템일 경우
            BEGIN
                SET @strGoodsUIDs = @strGoodsUIDs + ', '
                IF @iDuration_ > 0 -- 기간제 아이템
                BEGIN
                    IF EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin_ AND ItemID = @iGoodsID AND Period > 0)
                    BEGIN
                        UPDATE dbo.GoodsObjectList SET EndDate = EndDate + @iDuration_, Period = Period + @iDuration_
                        WHERE OwnerLogin = @strOwnerLogin_ AND ItemID = @iGoodsID AND Period > 0
                        IF @@ERROR <> 0
                            GOTO lbTran_Abort
                    END
                    ELSE
                    BEGIN
                        INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                        VALUES(@strOwnerLogin_, @strBuyerLogin_, @iGoodsID, @sdtNow, @sdtNow - 1, @sdtNow - 1, @iDuration_)
                        IF @@ERROR <> 0
                            GOTO lbTran_Abort
                    END
                END
                ELSE --일반 아이템
                BEGIN
                    INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                    VALUES(@strOwnerLogin_, @strBuyerLogin_, @iGoodsID, @sdtNow, @sdtNow, @sdtNow -1, -1)
                END
                
                -- 추가된 상품의 UID를 구함
                SELECT @iGoodsUID = @@identity
                IF @@ERROR <> 0
                    GOTO lbTran_Abort
                
                SET @strGoodsUIDs = @strGoodsUIDs + CAST(@iGoodsID AS nvarchar(10)) + ', ' + CAST(@iGoodsUID AS nvarchar(10))
                   + ', CONVERT( smalldatetime, ''' + CAST(@sdtNow AS nvarchar(20)) + ''' ), CONVERT( smalldatetime, ''' + CAST(@sdtNow AS nvarchar(20)) + ''' ), CONVERT( smalldatetime, ''' + CAST(DateAdd(d, @iDuration_, @sdtNow) AS nvarchar(20)) + ''' ), ' + CAST(@iDuration_ AS nvarchar(10))
                SET @iNumGoods = @iNumGoods + 1
            END --IF 셋트 아이템에서 그냥 아이템일경우
            ELSE -- godsend 셋트중에서 수량성 아이템일 경우
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
                    DECLARE @iDurationSetItemCount int --세트 아닌 수량성 아이템중에서 이미 같은 GoodsID가 있는경우 더한 값
                    SELECT @iDurationSetItemCount  = Duration FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin_ AND GoodsID = @iGoodsID
                    SET @iDurationSetItemCount = @iDurationNum + @iDurationSetItemCount
                    
                    SET @strDurationUIDs = @strDurationUIDs + ', ' -- godsend
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
        IF @iItemType = 0  --셋트중에서 일반 아이템일 경우
        BEGIN
            SET @strGoodsUIDs = @strGoodsUIDs + ', '
            IF @iDuration_ > 0 -- 기간제 아이템
            BEGIN
                IF EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin_ AND ItemID = @iGoodsID_ AND Period > 0)
                BEGIN
                    UPDATE dbo.GoodsObjectList SET EndDate = EndDate + @iDuration_, Period = Period + @iDuration_
                        WHERE OwnerLogin = @strOwnerLogin_ AND ItemID = @iGoodsID_ AND Period > 0
                    IF @@ERROR <> 0
                        GOTO lbTran_Abort
                END
                ELSE
                BEGIN
                    INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                        VALUES(@strOwnerLogin_, @strBuyerLogin_, @iGoodsID_, @sdtNow, @sdtNow - 1, @sdtNow - 1, @iDuration_)
                    IF @@ERROR <> 0
                        GOTO lbTran_Abort
                END
            END
            ELSE --일반 아이템
            BEGIN
                INSERT INTO dbo.GoodsObjectList(OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period)
                    VALUES(@strOwnerLogin_, @strBuyerLogin_, @iGoodsID_, @sdtNow, @sdtNow, @sdtNow -1, -1)
            END
            
            -- 추가된 상품의 UID를 구함
            SELECT @iGoodsUID = @@identity
            IF @@ERROR <> 0
                GOTO lbTran_Abort
            
            -- 결과를 돌려 주기 위한 SELECT 문을 만듦
            SET @iNumGoods = 1
            SET @strGoodsUIDs = ', ' + CAST(@iGoodsID_ AS nvarchar(10)) + ', ' + CAST(@iGoodsUID AS nvarchar(10))
                   + ', CONVERT( smalldatetime, ''' + CAST(@sdtNow AS nvarchar(20)) + ''' ), CONVERT( smalldatetime, ''' + CAST(@sdtNow AS nvarchar(20)) + ''' ), CONVERT( smalldatetime, ''' + CAST(DateAdd(d, @iDuration_, @sdtNow) AS nvarchar(20)) + ''' ), ' + CAST(@iDuration_ AS nvarchar(10))
        END
        ELSE
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


    -- Buyer의 Crystal을 변경
    SET @iMyCrystal = @iMyCrystal - @iCrystal

    IF @iMyCrystal < 0 
    BEGIN
        SET @iMyCrystal = 0
    END

    UPDATE dbo.DurationItemObjectList SET Duration = @iMyCrystal WHERE DurationItemUID = @iMyCrystalUID
    IF @@ERROR <> 0
        GOTO lbTran_Abort

    -- 해당 상품 정보의 hotcount 등 수정
    UPDATE dbo.GoodsInfoList SET HotCount = HotCount + 1 WHERE GoodsID = @iGoodsID_ AND Duration = @iDuration_
    IF @@ERROR <> 0
        GOTO lbTran_Abort

COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SELECT -9, @iMyCrystal
RETURN

lbTran_Success:
SET @strResultSQL = 'SELECT 0, ' +  CAST(@iMyCrystal AS nvarchar(10)) + ', ' + CAST(@iCrystalGoodsID AS nvarchar(10)) + ', ' + CAST(@iMyCrystalUID AS nvarchar(10)) + ', ' + CAST(@iCrystal AS nvarchar(10)) + ', ' + CAST(@iNumGoods AS nvarchar(10)) + @strGoodsUIDs + ', ' +  CAST(@iNumDurationSubSet AS nvarchar(10))  +  @strDurationUIDs
EXEC sp_executesql @strResultSQL
--PRINT @strResultSQL
RETURN


