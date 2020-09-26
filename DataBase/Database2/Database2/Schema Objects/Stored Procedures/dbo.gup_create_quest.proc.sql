CREATE PROCEDURE [dbo].[gup_create_quest]
	@iUnitUID_ [bigint],
	@iQuestID_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

--이미 진행중인 퀘스트가 있으면 새로 작성하지 않는다.
IF EXISTS(SELECT * FROM dbo.GQuests WITH(NOLOCK) WHERE UnitUID = @iUnitUID_ AND QuestID = @iQuestID_ )
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

DECLARE @sdtNow smalldatetime
SELECT  @sdtNow = GetDate()

BEGIN TRAN
    INSERT INTO dbo.GQuests(
                            UnitUID
,                           QuestID
,                           SubQuest0
,                           SubQuest1
,                           SubQuest2
,                           SubQuest3
,                           SubQuest4
,                           RegDate
                         )
        SELECT  @iUnitUID_
,               @iQuestID_
,               0
,               0
,               0
,               0
,               0
,               @sdtNow
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


