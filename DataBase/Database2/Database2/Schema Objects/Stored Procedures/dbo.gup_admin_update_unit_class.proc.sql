CREATE PROCEDURE [dbo].[gup_admin_update_unit_class]
    @iUnitUID   [bigint],
    @iUnitClass [tinyint],
    @iOK        [int] = 0
AS

SET NOCOUNT ON;

-- 해당 유닛이 없는 경우.
IF NOT EXISTS( SELECT * FROM dbo.GUnit WITH( NOLOCK ) WHERE UnitUID = @iUnitUID AND Deleted = 0 )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

BEGIN TRAN
    UPDATE dbo.GUnit WITH( ROWLOCK )
        SET UnitClass = @iUnitClass
			WHERE UnitUID = @iUnitUID
		IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT @iOK = -2	GOTO FAIL_TRAN	END

	IF EXISTS (SELECT * FROM [statistics].dbo.UnitStatistics WHERE UnitUID = @iUnitUID)
	BEGIN
	    UPDATE [statistics].dbo.UnitStatistics WITH( ROWLOCK )
	        SET UnitClass = @iUnitClass
				WHERE UnitUID = @iUnitUID

		IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT @iOK = -3	GOTO FAIL_TRAN	END

	END



COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


