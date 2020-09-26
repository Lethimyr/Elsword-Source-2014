CREATE PROCEDURE [dbo].[up_insert_pattern_for_cash]
	@strBuyerLogin_ [nvarchar](20),
	@strOwnerLogin_ [nvarchar](20),
	@iGoodsID_ [int],
	@iGoodsUID_ [int]
AS
exec dbo.up_log_set_proc_count 'up_insert_pattern_for_cash'

SET NOCOUNT ON

DECLARE     @bCash            bit,
            @bShowWeb         bit,
            @bPatternGoods    bit,
            @iGoodsID         int

-- buyer의 존재 확인
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strBuyerLogin_)
BEGIN
    RETURN -1
END

-- owner가 존재 하는지 확인
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strOwnerLogin_)
BEGIN
    RETURN -7
END

-- 존재하는 상품?, 상품의 가격을 알아 냄
IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_)
BEGIN
    RETURN -2
END
SELECT @bCash = Cash, @bShowWeb = ShowWeb FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iGoodsID_ AND Duration = -1

-- cash로 구입할 수 있는 상품인지 확인
IF @bCash = 0
BEGIN
    RETURN -3
END

-- 현재 판매 되고 있는 상품인지 확인
IF @bShowWeb = 0
BEGIN
    RETURN -8
END

-- Inventory가 full인지 확인(삭제 가능)
/*
IF (SELECT count(*) FROM dbo.GoodsObjectList WHERE OwnerLogin = @strOwnerLogin_) > 64
BEGIN
    SELECT -4, @iGP
    RETURN
END
*/

-- Pattern 상품인지 확인(GoodsID에 일단위 수가 존재하면 pattern 상품)
IF (@iGoodsID_ % 10) = 0
BEGIN
    RETURN -9
END

-- iGoodsUID의 상품을 가지고 있는지 확인
IF NOT EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE ItemUID = @iGoodsUID_ and OwnerLogin = @strOwnerLogin_ AND Period = -1)
BEGIN
    RETURN -6
END
SELECT @iGoodsID = ItemID FROM dbo.GoodsObjectList(NOLOCK) WHERE ItemUID = @iGoodsUID_ and OwnerLogin = @strOwnerLogin_ AND Period = -1

-- iGoodsUID의 상품과 구입하고자 하는 pattern 상품의 Base 상품이 같은지 확인
IF (@iGoodsID - (@iGoodsID%10)) <> (@iGoodsID_ - (@iGoodsID_%10))
BEGIN
    RETURN -11
END

BEGIN TRAN
    -- 이미 존재하는 상품의 ID를 수정
    UPDATE dbo.GoodsObjectList SET ItemID = @iGoodsID_, RegDate = GETDATE() WHERE ItemUID = @iGoodsUID_
    IF @@ERROR <> 0
        GOTO lbTran_Abort

    -- 해당 상품 정보의 hotcount 등 수정
    UPDATE dbo.GoodsInfoList SET HotCountWeb = HotCountWeb + 1 WHERE GoodsID = @iGoodsID_
    IF @@ERROR <> 0
        GOTO lbTran_Abort

COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
RETURN -10

lbTran_Success:
RETURN 0


