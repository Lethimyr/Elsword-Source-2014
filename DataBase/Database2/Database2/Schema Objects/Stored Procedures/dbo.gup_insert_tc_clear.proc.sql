CREATE PROCEDURE [dbo].[gup_insert_tc_clear]
    @iUnitUID	[bigint]
,   @iTCID		[int]
,   @sdtRegDate [smalldatetime]
,   @iOK        [int] = 0
AS

SET NOCOUNT ON;

-- 해당 유닛이 없는 경우.
IF NOT EXISTS( SELECT * FROM dbo.GUnit WITH( NOLOCK ) WHERE UnitUID = @iUnitUID AND Deleted = 0 )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

-- 이미 클리어하였다고 기록되어 있는 경우.
IF EXISTS( SELECT * FROM dbo.GTrainingCenter WITH( NOLOCK ) WHERE UnitUID = @iUnitUID AND TCID = @iTCID )
	BEGIN
		SELECT @iOK = -2
		GOTO END_PROC
	END


BEGIN TRAN

    INSERT INTO dbo.GTrainingCenter( UnitUID, TCID, RegDate )
		SELECT @iUnitUID, @iTCID, @sdtRegDate
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


