CREATE PROCEDURE [dbo].[gup_insert_inventory_size]
	@iUnitUID	bigint 
,	@iInventoryCategory tinyint
,	@iNumSlot	tinyint
,	@iOK		int = 0
AS
SET NOCOUNT ON;

DECLARE	@sdtNow	smalldatetime
SELECT	@sdtNow = GETDATE()

BEGIN TRAN

	INSERT INTO dbo.GItemInventorySize (UnitUID, InventoryCategory, NumSlot, RegDate)
		SELECT @iUnitUID, @iInventoryCategory, @iNumSlot, @sdtNow

	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT @iOK = -1	GOTO FAIL_TRAN	END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


