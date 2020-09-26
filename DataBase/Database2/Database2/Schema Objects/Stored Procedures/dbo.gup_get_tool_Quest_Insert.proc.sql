/*	sutamtwo
	OK값

	-1 : 삭제 에러
	-2 : 삭제 기록 작성 에러
*/

CREATE PROCEDURE [dbo].[gup_get_tool_Quest_Insert]
	@iQuestID_	[bigint]
,	@iUnitUID_	[bigint]
,	@iOK		[int] = 0

AS
SET NOCOUNT ON;

BEGIN TRAN
	IF EXISTS (SELECT * FROM game01.dbo.GQuests WITH (NOLOCK) WHERE UnitUID = @iUnitUID_ AND QuestID = @iQuestID_) or EXISTS (SELECT * FROM game01.dbo.GQuests_Complete WITH (NOLOCK) WHERE UnitUID = @iUnitUID_ AND QuestID = @iQuestID_)
		BEGIN    SELECT @iOK = -1    GOTO FAIL_TRAN    END
		
	ELSE 
		INSERT INTO game01.dbo.GQuests (UnitUID, QuestID, SubQuest0, SubQuest1, SubQuest2, SubQuest3, SubQuest4, regdate)
			select @iUnitUID_, @iQuestID_, 0,0,0,0,0,getdate()
		IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
			BEGIN    SELECT @iOK = -2    GOTO FAIL_TRAN    END

COMMIT TRAN


GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


