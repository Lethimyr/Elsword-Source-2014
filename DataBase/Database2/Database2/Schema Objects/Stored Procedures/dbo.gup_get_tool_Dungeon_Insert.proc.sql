/*	sutamtwo
	OK값

	-1 : 삭제 에러
	-2 : 삭제 기록 작성 에러
*/

CREATE PROCEDURE [dbo].[gup_get_tool_Dungeon_Insert]
	@iDungeonID_	[bigint]
,	@iUnitUID_		[bigint]
,	@iOK			[int] = 0

AS
SET NOCOUNT ON;

BEGIN TRAN
	IF EXISTS (SELECT * FROM game01.dbo.GDungeonClear WITH (NOLOCK) WHERE UnitUID = @iUnitUID_ AND GameMode = @iDungeonID_)
		BEGIN    SELECT @iOK = -1    GOTO FAIL_TRAN    END
		
	ELSE 
		INSERT INTO game01.dbo.GDungeonClear (UnitUID, GameMode, MaxScore,MaxTotalRank,regdate)
			SELECT @iUnitUID_, @iDungeonID_, 0,0,getdate()
		IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
			BEGIN    SELECT @iOK = -2    GOTO FAIL_TRAN    END

COMMIT TRAN


GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


