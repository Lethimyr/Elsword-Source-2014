CREATE PROCEDURE [dbo].[up_admin_insert_gp_duration_item]
	@strBuyerLogin_ [nvarchar](20),
	@strOwnerLogin_ [nvarchar](20),
	@iGoodsID_ [int]
AS
exec dbo.up_log_set_proc_count 'up_admin_insert_gp_duration_item'

SET NOCOUNT ON

DECLARE   @bCash                bit,
          @iGoodsUID            int,
          @bSubSetGoods         bit,
          @iSetGoodsNum         int,
          @iNumGoods            int,
          @iDuration            int,  --godsend --일반 아이템의 수량을 저장할때
          @iDurationUID         int,  --godsend  --DurationItemObjectList의 UID저장
          @iNumDurationSubSet   int  -- godsend 수량성 아이템의 셋트 갯수

-- buyer의 존재, buyer의 GP를 구함
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strBuyerLogin_)
BEGIN
    SELECT -1
    RETURN
END

-- owner가 존재 하는지 확인
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strOwnerLogin_)
BEGIN
    SELECT -5
    RETURN
END

-- 존재하는 상품?, 상품정보 알아 냄
IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_)
BEGIN
    SELECT -2
    RETURN
END
SELECT @bCash = Cash, 
       @bSubSetGoods = IsSubSetGoods, 
       @iSetGoodsNum = SetGoodsNum,
       @iDuration = Duration --godsend
FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_

-- GP로 구입할 수 있는 상품인지 확인
IF @bCash = 1
BEGIN
    SELECT -3
    RETURN
END

IF @iDuration <= 0 
BEGIN
    SELECT -4
    RETURN
END

SET NOCOUNT ON


BEGIN TRAN
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
            IF @iDurationNum > 0  --셋트중에서 일반 아이템일 경우
              BEGIN
                IF NOT EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin_ AND  GoodsID = @iGoodsID )
                --세트 아이템이고 수량성 아이템인데,  GoodsID를 가지고 있지 않은 경우 
                BEGIN
          INSERT INTO dbo.DurationItemObjectList ( OwnerLogin, BuyerLogin, GoodsID, Duration, RegDate ) 
                   VALUES ( @strOwnerLogin_, @strBuyerLogin_, @iGoodsID, @iDurationNum, GETDATE() )
                   IF @@ERROR <> 0
                       GOTO lbTran_Abort

                    SET @iNumDurationSubSet = @iNumDurationSubSet + 1
                  END
                  ELSE
--세트 아이템이고 수량성 아이템인데,  GoodsID를 이미 가지고 있는 경우 
                  BEGIN
                      DECLARE @iDurationSetItemCount int --세트 아닌 수량성 아이템중에서 이미 같은 GoodsID가 있는경우 더한 값
             SELECT @iDurationSetItemCount  = Duration FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strOwnerLogin_ AND GoodsID = @iGoodsID
                      SET @iDurationSetItemCount = @iDurationNum + @iDurationSetItemCount

                       UPDATE dbo.DurationItemObjectList 
                       SET Duration = @iDurationSetItemCount WHERE OwnerLogin = @strOwnerLogin_  AND GoodsID = @iGoodsID 
                        IF @@ERROR <> 0
                             GOTO lbTran_Abort
                     
                        SET @iNumDurationSubSet = @iNumDurationSubSet + 1
                  END --else
              END --else
     ELSE
     BEGIN
         GOTO lbTran_Abort
     END
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
    END --godsend

                -- 결과를 돌려 주기 위한 SELECT 문을 만듦
                SET @iNumDurationSubSet = 1
          END --godsend 일반상품의 수량성 아이템
          ELSE
          BEGIN
    GOTO lbTran_Abort
          END
    END -- else 셋트상품이 아닌 일반상품

    INSERT INTO dbo.AdminInsertDurationItem ( OwnerLogin, BuyerLogin, GoodsID, RegDate ) 
          VALUES ( @strOwnerLogin_, @strBuyerLogin_, @iGoodsID_, GETDATE() )


COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SELECT -9
RETURN

lbTran_Success:
SELECT 0
RETURN


