CREATE PROCEDURE [dbo].[gup_update_item_position]
    @iItemUID           [bigint],
    @iInventoryCategory [int],
    @iSlotID            [int],
    @iOK                [int] = 0
AS

SET NOCOUNT ON;

-- 해당 아이템이 없는 경우.
IF NOT EXISTS( SELECT * FROM dbo.GItem WITH( NOLOCK ) WHERE ItemUID = @iItemUID AND Deleted = 0 )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

BEGIN TRAN
    UPDATE dbo.GItem WITH( ROWLOCK )
        SET InventoryCategory	= @iInventoryCategory
,			SlotID				= @iSlotID
        WHERE ItemUID = @iItemUID
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN
        SELECT @iOK = -3
        GOTO FAIL_TRAN
    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


