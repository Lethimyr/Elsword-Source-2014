CREATE PROCEDURE [dbo].[removed_gup_update_inventory_slot]
    @iUnitUID               [bigint],
    @iInventoryCategory     [tinyint],
    @iSlotID                [tinyint],
    @iItemUID               [bigint],
    @iQuantity              [smallint],
    @iOK                    [int] = 0
AS

SET NOCOUNT ON;

-- 동일한 uid를 가지는 아이템이 이미 테이블에 존재하는 경우
IF EXISTS( SELECT * FROM dbo.GItemInventory(NOLOCK) WHERE ItemUID = @iItemUID AND InventoryCategory != @iInventoryCategory AND SlotID != @iSlotID  )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

BEGIN TRAN
    IF @iItemUID = 0
    BEGIN
        UPDATE dbo.GItemInventory WITH( ROWLOCK )
            SET ItemUID = NULL, Quantity = 0
            WHERE UnitUID = @iUnitUID AND InventoryCategory = @iInventoryCategory AND SlotID = @iSlotID
    END
    ELSE
    BEGIN
        UPDATE dbo.GItemInventory WITH( ROWLOCK )
            SET ItemUID = @iItemUID, Quantity = @iQuantity
            WHERE UnitUID = @iUnitUID AND InventoryCategory = @iInventoryCategory AND SlotID = @iSlotID
    END
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


