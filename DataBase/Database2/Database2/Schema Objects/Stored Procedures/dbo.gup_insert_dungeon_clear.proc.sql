CREATE PROCEDURE [dbo].[gup_insert_dungeon_clear]
    @iUnitUID   [bigint],
    @iDungeonID [int],
	@iMaxScore	[int],
	@iMaxTotalRank	[tinyint],
    @sdtRegDate [smalldatetime],
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
	-- 이미 클리어하였다고 기록되어 있는 경우.
	IF EXISTS( SELECT * FROM dbo.GDungeonClear WITH( NOLOCK ) WHERE UnitUID = @iUnitUID AND GameMode = @iDungeonID )
		BEGIN
			UPDATE dbo.GDungeonClear WITH (UPDLOCK)
				SET MaxScore = @iMaxScore, MaxTotalRank = @iMaxTotalRank
					WHERE UnitUID = @iUnitUID AND GameMode = @iDungeonID
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -2	GOTO	FAIL_TRAN
			END 
		END

	ELSE
		BEGIN
			INSERT INTO dbo.GDungeonClear( UnitUID, GameMode, MaxScore, MaxTotalRank, RegDate )
			SELECT @iUnitUID, @iDungeonID, @iMaxScore, @iMaxTotalRank, @sdtRegDate
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
			BEGIN
				SELECT @iOK = -3
				GOTO FAIL_TRAN
			END    
		END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


