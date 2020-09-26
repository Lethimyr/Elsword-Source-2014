CREATE PROCEDURE [dbo].[gup_delete_quest]
	@iUnitUID_ [bigint],
	@iQuestID_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

--퀘스트가 없으면 지우지 않는다.
IF NOT EXISTS(SELECT * FROM dbo.GQuests WITH(NOLOCK) WHERE UnitUID = @iUnitUID_ AND QuestID = @iQuestID_ )
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

BEGIN TRAN
    DELETE dbo.GQuests
        WHERE UnitUID = @iUnitUID_ AND QuestID = @iQuestID_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


