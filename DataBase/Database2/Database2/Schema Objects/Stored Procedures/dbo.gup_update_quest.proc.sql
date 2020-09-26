CREATE PROCEDURE [dbo].[gup_update_quest]
    @iUnitUID   [bigint],
    @iQuestID   [int],
	@iSubQuest0	[tinyint],
	@iSubQuest1	[tinyint],
	@iSubQuest2	[tinyint],
	@iSubQuest3	[tinyint],
	@iSubQuest4	[tinyint],
    @iOK        [int] = 0
AS

SET NOCOUNT ON;

-- 해당 퀘스트가 없는 경우.
IF NOT EXISTS( SELECT * FROM dbo.GQuests WITH( NOLOCK ) WHERE UnitUID = @iUnitUID AND QuestID = @iQuestID )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

BEGIN TRAN
    UPDATE dbo.GQuests WITH( ROWLOCK )
        SET SubQuest0 = @iSubQuest0, SubQuest1 = @iSubQuest1, SubQuest2 = @iSubQuest2, 
			SubQuest3 = @iSubQuest3, SubQuest4 = @iSubQuest4
        WHERE UnitUID = @iUnitUID AND QuestID = @iQuestID
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


