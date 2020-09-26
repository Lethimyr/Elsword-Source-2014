/*	sutamtwo
	OK값

	-1 : 삭제 에러
	-2 : 삭제 기록 작성 에러
*/
CREATE PROCEDURE [dbo].[gup_get_tool_delete_dungeon]
	@iDungeonID_	[bigint]
,	@iUnitUID_		[bigint]
,	@iOK			[int] = 0


AS
SET NOCOUNT ON;

BEGIN TRAN
	
	DELETE FROM game01.dbo.GDungeonClear
		WHERE unitUID = @iUnitUID_ and GameMode = @iDungeonID_

    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -1    GOTO FAIL_TRAN    END

COMMIT TRAN


GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


