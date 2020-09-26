CREATE PROCEDURE [dbo].[gup_insert_item_Code]
    @iItemCode      [int]
,	@strItemName	nvarchar(16)
,   @iPeriod		[tinyint]
,	@iEndurance		[int],
    @iOK            [int] = 0

AS

SET NOCOUNT ON;

BEGIN TRAN

	INSERT INTO dbo.GItemCode (itemcode, itemname, period, endurance)
		SELECT @iItemCode, @strItemName, @iPeriod, @iEndurance

		IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
			BEGIN
				SELECT @iOK = -11	GOTO FAIL_TRAN
			END
    
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


