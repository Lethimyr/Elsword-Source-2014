CREATE PROCEDURE [dbo].[gup_update_unit_info]
    @iUnitUID   	[bigint],
    @iExp       	[int],
    @iLevel     	[int],
    @iGamePoint 	[int],
	@iVSPoint		[int],
	@iVSPointMax	[int],
	@iSPoint		[int],
	@iWin			[int],
	@iLose			[int],
	@iMapID			[int],
	@iSpirit		[smallint],
	@iSpiritFlag	[bit],
    @iOK        	[int] = 0
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
--	IF DateDiff ( day, @LastDate, @sdtNow ) > 0
--	BEGIN
--		UPDATE dbo.GUnit WITH( ROWLOCK ) SET PlayDayCnt = PlayDayCnt + 1 WHERE UnitUID = @iUnitUID
--	END

    UPDATE dbo.GUnit WITH( ROWLOCK )
        SET Exp = Exp + @iExp, [Level] = @iLevel, GamePoint = GamePoint + @iGamePoint, VSPoint = VSPoint + @iVSPoint,
			VSPointMax = VSPointMax + @iVSPointMax,
			SPoint = SPoint + @iSPoint, Win = @iWin, Lose = @iLose, LastPosition = @iMapID--,
--			LastDate = @sdtNow
        WHERE UnitUID = @iUnitUID
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN
        SELECT @iOK = -2
        GOTO FAIL_TRAN
    END

-- 근성도 저장
	UPDATE dbo.GSpirit WITH (UPDLOCK)
		SET Spirit = @iSpirit
			WHERE unitUID = @iunitUID
	    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
			BEGIN
				SELECT @iOK = -3
				GOTO FAIL_TRAN
			END

--	IF	(@iSpiritFlag = 1 AND (SELECT Flag FROM dbo.GSpirit WHERE UnitUID = @iunitUID)=0 ) 
--	BEGIN
--		UPDATE dbo.GSpirit WITH (UPDLOCK)
--			SET Flag = 1
--				WHERE unitUID = @iunitUID
--	END


COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


