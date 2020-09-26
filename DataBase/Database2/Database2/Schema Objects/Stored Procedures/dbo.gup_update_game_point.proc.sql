CREATE PROCEDURE [dbo].[gup_update_game_point]
    @iUnitUID   [bigint],
    @iGamePoint [int],
	@iVSPoint	[int],
    @iOK        [int] = 0
AS

SET NOCOUNT ON;

-- 해당 유닛이 없는 경우.
IF NOT EXISTS( SELECT * FROM dbo.GUnit WITH( NOLOCK ) WHERE UnitUID = @iUnitUID AND Deleted = 0 )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

DECLARE @iCurrentGamePoint int
DECLARE @iCurrentVSPoint int

BEGIN TRAN
    UPDATE dbo.GUnit WITH( ROWLOCK )
        SET @iCurrentGamePoint = GamePoint = GamePoint + @iGamePoint, @iCurrentVSPoint = VSPoint = VSPoint + @iVSPoint
        WHERE UnitUID = @iUnitUID
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN
        SELECT @iOK = -2
        GOTO FAIL_TRAN
    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @iCurrentGamePoint, @iCurrentVSPoint


