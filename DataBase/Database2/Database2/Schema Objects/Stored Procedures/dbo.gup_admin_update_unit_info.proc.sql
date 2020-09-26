CREATE PROCEDURE [dbo].[gup_admin_update_unit_info]
    @iUnitUID   	[bigint],
    @iExp       	[int],
    @iLevel     	[int],
--  @iGamePoint 	[int],
--	@iVSPoint		[int],
--	@iVSPointMax	[int],
--	@iWin			[int],
--	@iLose			[int],
--	@iSeceder		[int],
    @iOK        	[int] = 0
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
        SET Exp = @iExp, [Level] = @iLevel--, GamePoint = @iGamePoint, VSPoint = @iVSPoint,
			--VSPointMax = @iVSPointMax,
			--Win = @iWin, Lose = @iLose, Seceder = @iSeceder
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
SELECT @iOK


