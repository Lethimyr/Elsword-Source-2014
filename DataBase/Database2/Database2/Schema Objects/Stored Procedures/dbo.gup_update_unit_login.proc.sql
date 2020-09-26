CREATE PROCEDURE [dbo].[gup_update_unit_login]
    @iUnitUID   [bigint],
    @iOK        [int] = 0
AS

SET NOCOUNT ON;

DECLARE 
	@sdtNow smalldatetime,
	@LastDate smalldatetime

SELECT @sdtNow = GetDate()

SELECT @LastDate = LastDate FROM dbo.GUnit WHERE UnitUID = @iUnitUID

-- 해당 유닛이 없는 경우.
IF NOT EXISTS( SELECT * FROM dbo.GUnit WITH( NOLOCK ) WHERE UnitUID = @iUnitUID AND Deleted = 0 )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

BEGIN TRAN
	IF DateDiff ( day, @LastDate, @sdtNow ) > 0
	BEGIN
		UPDATE dbo.GUnit WITH( ROWLOCK ) SET PlayDayCnt = PlayDayCnt + 1 WHERE UnitUID = @iUnitUID
	END

	UPDATE dbo.GUnit WITH( ROWLOCK ) SET 
		LoginCount = LoginCount + 1, LastDate = @sdtNow 
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


