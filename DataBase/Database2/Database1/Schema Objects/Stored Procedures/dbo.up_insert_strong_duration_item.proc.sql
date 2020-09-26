CREATE PROCEDURE [dbo].[up_insert_strong_duration_item]
	@strLogin_ [nvarchar](20),
	@iGoodsID_ [int]
AS
exec dbo.up_log_set_proc_count 'up_insert_strong_duration_item'

SET NOCOUNT ON

DECLARE     @iGP                int,
            @iPrice             int,
            @bCash              bit,
            @bShowWeb           bit,
            @iGoodsUID          int,
            @bSubSetGoods       bit,
            @iSetGoodsNum       int,
            @iNumGoods          int,
            @strGoodsUIDs       nvarchar(100),
            @strResultSQL       nvarchar(200),
            @strDurationUIDs    nvarchar(100), --godsend 리턴값을 할때 사용
            @iDuration	        int,  --godsend --일반 아이템의 수량을 저장할때
            @iDurationUID       int,  --godsend  --DurationItemObjectList의 UID저장
            @iNumDurationSubSet int  -- godsend 수량성 아이템의 셋트 갯수

-- buyer의 존재, buyer의 GP를 구함
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strLogin_)
BEGIN
    SELECT -1, 0, 0
    RETURN
END

-- 존재하는 상품?, 상품정보 알아 냄
IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_)
BEGIN
    SELECT -2, 0, 0
    RETURN
END
SELECT   @bCash = Cash,  
         @bShowWeb = ShowWeb, 
         @bSubSetGoods = IsSubSetGoods, 
         @iSetGoodsNum = SetGoodsNum,  
         @iDuration	= Duration --godsend
FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_

-- GP로 구입할 수 있는 상품인지 확인
--IF @bCash = 1
--BEGIN
--    SELECT -3, 0, 0
--    RETURN
--END

IF @iDuration <= 0 
BEGIN
   SELECT -4, 0, 0
   RETURN
END


-- 현재 판매 되고 있는 상품인지 확인
/*
IF @bShowWeb = 0
BEGIN
    SELECT -5, 0, 0
    RETURN
END
*/
-- Inventory가 full인지 확인(삭제 가능)
--IF( @bSubSetGoods = 1 AND @iSetGoodsNum > -1 ) -- 세트 상품인 경우
--    SET @iNumGoods = (SELECT count(*) FROM dbo.GoodsInfoList(NOLOCK)
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

-- Pattern 상품인지 확인(GoodsID에 일단위 수가 존재하면 pattern 상품)
IF (@iGoodsID_ % 10) <> 0
BEGIN
    SELECT -6, 0, 0
    RETURN
END

BEGIN TRAN
    IF @bSubSetGoods = 1 AND @iSetGoodsNum > -1   -- 세트 상품인 경우
    BEGIN
      SELECT -7, 0, 0
      RETURN
    END --IF 셋트일경우
    ELSE                    -- 일반 상품인 경우
     BEGIN
           IF NOT EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND  GoodsID = @iGoodsID_ )
           BEGIN
--세트아닌 수량성 아이템중에서 같은 아이템이 없는 경우
               INSERT INTO dbo.DurationItemObjectList ( OwnerLogin, BuyerLogin, GoodsID, Duration, RegDate ) 
                     VALUES ( @strLogin_, @strLogin_, @iGoodsID_, @iDuration, GETDATE() )
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
	   SELECT @iDurationItemCount  = Duration FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND GoodsID = @iGoodsID_

                SET @iDurationItemCount = @iDuration + @iDurationItemCount

                   UPDATE dbo.DurationItemObjectList 
                   SET Duration = @iDurationItemCount  WHERE OwnerLogin = @strLogin_  AND GoodsID = @iGoodsID_ 
               IF @@ERROR <> 0
                    GOTO lbTran_Abort

               -- 추가된 상품의 UID를 구함
               SELECT @iDurationUID = DurationItemUID FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND GoodsID = @iGoodsID_
                IF @@ERROR <> 0
                     GOTO lbTran_Abort
	END --godsend

                -- 결과를 돌려 주기 위한 SELECT 문을 만듦
--             SET @iNumDurationSubSet = 1
--                SET @strDurationUIDs = ', ' +  CAST(@iGoodsID_ AS nvarchar(10)) + ', ' + CAST(@iDurationUID AS nvarchar(10)) + ', ' +  CAST(@iDuration  AS nvarchar(10))
          END --godsend 일반상품의 수량성 아이템

    -- 해당 상품 정보의 hotcount 등 수정
    UPDATE dbo.GoodsInfoList SET HotCount = HotCount + 1 WHERE GoodsID = @iGoodsID_
    IF @@ERROR <> 0
        GOTO lbTran_Abort

COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SELECT -9, 0, 0
RETURN

lbTran_Success:
SET @strResultSQL = 'SELECT 0, ' + CAST(@iDurationUID AS nvarchar(10)) + ', ' +  CAST(@iDuration  AS nvarchar(10))
EXEC( @strResultSQL )
--PRINT @strResultSQL
RETURN


