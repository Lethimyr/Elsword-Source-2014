CREATE PROCEDURE [dbo].[up_buy_cash_item_rollback]
	@iItemUID_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

IF NOT EXISTS(SELECT * FROM dbo.CashItemList(NOLOCK) WHERE ItemUID = @iItemUID_)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

BEGIN TRAN
    DELETE FROM dbo.CashItemList WHERE ItemUID = @iItemUID_
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

    DELETE FROM dbo.CashItemDisplayList WHERE ItemUID = @iItemUID_
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


